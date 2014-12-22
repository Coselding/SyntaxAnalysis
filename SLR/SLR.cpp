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
        cout << "�����ļ������� fin.txt."  << endl;
        return 1;
    }
    ofstream fout("fout.txt");
    if(!fout.is_open())
    {
        cout << "�޷�������ļ� fout.txt." << endl;
        return 1;
    }
    //���봮
    string s;
    fin >> s;
    cout << "�ɹ���ȡ����������" << endl << s << endl;
    int wid = s.length()+1;
    s.push_back('$');

    //״̬ջ������ջ
    vector<int> statusStack;
    vector<char> symbolStack;
    statusStack.push_back(0);
    symbolStack.push_back('$');

    // ջ���͵�ǰ����
    int top = 0;
    char cur = '\0';
    auto ip = s.begin();

    //���ͷ
    fout << left << setw(wid+10) << "ջ" << right << setw(wid) << "����" << "    " << "��������" << endl;
    do
    {
        // �����ǰջ�͵�ǰʣ������
        stringstream tmp;
        for (vector<int>::size_type i = 0; i < statusStack.size(); ++i)
            tmp << symbolStack.at(i) << statusStack.at(i);
        string str1;
        tmp >> str1;
        string str2(ip, s.end());
        fout << left << setw(wid+10) << str1 << right << setw(wid) << str2 << "    ";

        // ջ���͵�ǰ�������
        top = statusStack.back();
        cur = *ip;

        // ��ʶ�������ֱ任
        if (isalpha(cur))
            cur = 'i';
        else if (isdigit(cur))
            cur = 'n';

        // ����任
        int x = top;
        int y;
        try
        {
            y = index.at(cur);
        }
        catch (out_of_range)
        {
            fout << "�����ַ��Ƿ���" << endl;
            break;
        }
        int val = action.at(x).at(y);

        // ����
        if (val == INT_MAX)
        {
            fout << "acc" << endl;
            break;
        }
        // �ƽ�
        else if (val > 0)
        {
            fout << "Shift " << val << endl;
            symbolStack.push_back(cur);
            statusStack.push_back(val);
            ++ip;
        }
        // ��Լ
        else if (val < 0)
        {
            string production = G.at(abs(val));
            int len = production.length() - 3;
            while (len--)    //��ջ
            {
                statusStack.pop_back();
                symbolStack.pop_back();
            }
            int topS = statusStack.back();
            char curA = production.at(0);
            int xx = topS;
            int yy = index.at(curA);
            //ѹ״̬ջ�ͷ���ջ
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
    cout << endl << "�������������� fout.txt." << endl;
    return 0;
}
