#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <climits>
#include <algorithm>
#include <iterator>
#include <stdexcept>

using namespace std;

vector<string> G = {"S->E", "E->E+T", "E->E-T", "E->T", "T->T*F", "T->T/F", "T->F", "F->i", "F->(E)", "F->n"};

map<char, int> index = {{'+', 0}, {'-', 1}, {'*', 2}, {'/', 3}, {'(', 4}, {')', 5}, {'i', 6}, {'n', 7}, {'$', 8}, {'E', 0}, {'T', 1}, {'F', 2}};

vector<vector<int> > action =
{
    { 0 ,  0,  0,  0 , +5,  0 , +4, +6,  0},
    { +7, +8,  0,  0 ,  0,  0 ,  0,  0,  INT_MAX},
    { -3, -3, +9, +10,  0, -3 ,  0,  0, -3},
    { -6, -6, -6, -6 ,  0, -6 ,  0,  0, -6},
    { -7, -7, -7, -7 ,  0, -7 ,  0,  0, -7},
    { 0 ,  0,  0,  0 , +5,  0 , +4, +6,  0},
    { -9, -9, -9, -9 ,  0, -9 ,  0,  0, -9},
    { 0 ,  0,  0,  0 , +5,  0 , +4, +6,  0},
    { 0 ,  0,  0,  0 , +5,  0 , +4, +6,  0},
    { 0 ,  0,  0,  0 , +5,  0 , +4, +6,  0},
    { 0 ,  0,  0,  0 , +5,  0 , +4, +6,  0},
    { +7, +8,  0,  0 ,  0, +16,  0,  0,  0},
    { -1, -1, +9, +10,  0, -1 ,  0,  0, -1},
    { -2, -2, +9, +10,  0, -2 ,  0,  0, -2},
    { -4, -4, -4, -4 ,  0, -4 ,  0,  0, -4},
    { -5, -5, -5, -5 ,  0, -5 ,  0,  0, -5},
    { -8, -8, -8, -8 ,  0, -8 ,  0,  0, -8}
};

vector<vector<int> > goTo =
{
    {1 , 2 , 3},
    {0 , 0 , 0},
    {0 , 0 , 0},
    {0 , 0 , 0},
    {0 , 0 , 0},
    {11, 2 , 3},
    {0 , 0 , 0},
    {0 , 12, 3},
    {0 , 13, 3},
    {0 , 0 , 14},
    {0 , 0 , 15},
    {0 , 0 , 0},
    {0 , 0 , 0},
    {0 , 0 , 0},
    {0 , 0 , 0},
    {0 , 0 , 0},
    {0 , 0 , 0}
};


int main()
{
    ifstream fin("fin.txt");
    if(!fin.is_open())
    {
        cout << "输入文件不存在 fin.txt."  << endl;
        return 1;
    }
    ofstream fout("fout.txt");
    if(!fout.is_open())
    {
        cout << "无法打开输出文件 fout.txt." << endl;
        return 1;
    }
    //输入串
    string s;
    fin >> s;
    cout << "成功读取待分析串：" << endl << s << endl;
    int wid = s.length()+1;
    s.push_back('$');

    //状态栈、符号栈
    vector<int> statusStack;
    vector<char> symbolStack;
    statusStack.push_back(0);
    symbolStack.push_back('$');

    // 栈顶和当前输入
    int top = 0;
    char cur = '\0';
    auto ip = s.begin();

    //输出头
    fout << left << setw(wid+10) << "栈" << right << setw(wid) << "输入" << "    " << "分析动作" << endl;
    do
    {
        // 输出当前栈和当前剩余输入
        stringstream tmp;
        for (vector<int>::size_type i = 0; i < statusStack.size(); ++i)
            tmp << symbolStack.at(i) << statusStack.at(i);
        string str1;
        tmp >> str1;
        string str2(ip, s.end());
        fout << left << setw(wid+10) << str1 << right << setw(wid) << str2 << "    ";

        // 栈顶和当前输入符号
        top = statusStack.back();
        cur = *ip;

        // 标识符及数字变换
        if (isalpha(cur))
            cur = 'i';
        else if (isdigit(cur))
            cur = 'n';

        // 坐标变换
        int x = top;
        int y;
        try
        {
            y = index.at(cur);
        }
        catch (out_of_range)
        {
            fout << "输入字符非法！" << endl;
            break;
        }
        int val = action.at(x).at(y);

        // 接受
        if (val == INT_MAX)
        {
            fout << "acc" << endl;
            break;
        }
        // 移进
        else if (val > 0)
        {
            fout << "Shift " << val << endl;
            symbolStack.push_back(cur);
            statusStack.push_back(val);
            ++ip;
        }
        // 归约
        else if (val < 0)
        {
            string production = G.at(abs(val));
            int len = production.length() - 3;
            while (len--)    //弹栈
            {
                statusStack.pop_back();
                symbolStack.pop_back();
            }
            int topS = statusStack.back();
            char curA = production.at(0);
            int xx = topS;
            int yy = index.at(curA);
            //压状态栈和符号栈
            symbolStack.push_back(curA);
            statusStack.push_back(goTo.at(xx).at(yy));
            fout << "reduce by " << production << endl;
        }
        else
        {
            fout << "error" << endl;
            break;
        }
    }
    while (true);
    cout << endl << "分析结果已输出至 fout.txt." << endl;
    return 0;
}
