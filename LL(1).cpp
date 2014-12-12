#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

using namespace std;

// 预测分析表
vector<vector<string> > table(5, vector<string>(9));
// 文法的产生式
vector<string> G = {"E->TA", "A->+TA", "A->-TA", "A->e", "T->FB", "B->*FB", "B->/FB", "B->e", "F->i", "F->(E)", "F->n"};
// 文法符号到下标转换
map<char, int> index = {{'E', 0}, {'A', 1}, {'T', 2}, {'B', 3}, {'F', 4}, {'i', 0}, {'n', 1}, {'+', 2}, {'-', 3}, {'*', 4}, {'/', 5}, {'(', 6}, {')', 7}, {'$', 8}, {'e', 9}};
// 终结符
string terminal("in+-*/()$");
// 非终结符
string nonTerminal("EATBF");
// 产生式右部的first集
vector<string> First = {"i(n", "+", "-", "e", "i(n", "*", "/", "e", "i", "(", "n"};
// 非终结符的follow集
vector<string> Follow = {"$)", "$)", "+-$)", "+-$)", "*/+-$)"};


void analysis(void);

int main()
{
    //算法4.2 构造预测分析表
    for (auto itG = G.begin(), itFirst = First.begin(); itG != G.end() && itFirst != First.end(); ++itG, ++itFirst)
    {
        int x = index.at(*(itG->begin()));
        for (auto first = itFirst->begin(); first != itFirst->end(); ++first)
        {
            if (*first != 'e')
            {
                int y = index.at(*first);
                table.at(x).at(y) = *itG;
            }
            else
            {
                for (auto follow = Follow.at(x).begin(); follow != Follow.at(x).end(); ++follow)
                {
                    int y = index.at(*follow);
                    table.at(x).at(y) = *itG;
                }
            }
        }
    }

    // 输出预测分析表
    cout << "预测分析表：" << endl;
    // 输出终结符
    for (string::size_type i = 0; i < terminal.size(); ++i)
    {
        cout << '\t' << terminal[i];
    }
    cout << endl;
    // 输出非终结符
    for (string::size_type x = 0; x < nonTerminal.size(); ++x)
    {
        cout << nonTerminal[x];
        // 输出产生式
        for (string::size_type y = 0; y < table.at(x).size(); ++y)
            cout << '\t' << table.at(x).at(y);
        cout << endl;
    }
    cout << endl;

    analysis();
    return 0;
}

// 预测分析过程
void analysis(void)
{
    cout << "请输入待分析符号串：" << endl;

    //输入缓冲区
    string s;
    cin >> s;
    s.push_back('$');

    //分析栈
    vector<char> analyStack;
    analyStack.push_back('$');
    analyStack.push_back('E');

    // 栈顶和当前输入
    char top = '\0', cur = '\0';
    auto ip = s.begin();
    cout << endl << left << setw(10) << "栈" << right << setw(20) << "输入" << "\t" << "输出" << endl;

    do
    {
        // 输出当前栈和当前剩余输入
        string str1(analyStack.begin(), analyStack.end());
        string str2(ip, s.end());
        cout << left << setw(10) << str1 << right << setw(20) << str2 << "\t";

        top = analyStack.back();
        cur = *ip;

        // 标识符及数字变换
        if (isalpha(cur))
            cur = 'i';
        else if (isdigit(cur))
                cur = 'n';

        // 栈顶是终结符号或$
        if (terminal.find(top) != terminal.npos || top == '$')
        {
            if (top == cur)
            {
                analyStack.pop_back();
                ++ip;
                cout << endl;
            }
            else
            {
                cout << "error" << endl;
                return;
            }
        }
        else
        {
            int x = index.at(top);
            int y = index.at(cur);
            // 产生式
            string production = table.at(x).at(y);
            if (!production.empty())
            {
                analyStack.pop_back();
                string expr(production.begin() + 3, production.end());
                if (expr == "e")    // 空产生式
                    expr = "";
                // 逆序压栈
                for (auto iter = expr.rbegin(); iter != expr.rend(); ++iter)
                    analyStack.push_back(*iter);
                // 输出产生式
                cout << production << endl;
            }
            else
            {
                cout << "error" << endl;
                return;
            }
        }
    }
    while (top != '$');
}
