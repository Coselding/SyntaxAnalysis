#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

// Ԥ�������
vector<vector<string> > table(5, vector<string>(9));
// �ķ��Ĳ���ʽ
vector<string> G = {"E->TA", "A->+TA", "A->-TA", "A->e", "T->FB", "B->*FB", "B->/FB", "B->e", "F->i", "F->(E)", "F->n"};
// �ķ����ŵ��±�ת��
map<char, int> index = {{'E', 0}, {'A', 1}, {'T', 2}, {'B', 3}, {'F', 4}, {'i', 0}, {'n', 1}, {'+', 2}, {'-', 3}, {'*', 4}, {'/', 5}, {'(', 6}, {')', 7}, {'$', 8}, {'e', 9}};
// �ս��
string terminal("in+-*/()$");
// ���ս��
string nonTerminal("EATBF");
// ����ʽ�Ҳ���first��
vector<string> First = {"i(n", "+", "-", "e", "i(n", "*", "/", "e", "i", "(", "n"};
// ���ս����follow��
vector<string> Follow = {"$)", "$)", "+-$)", "+-$)", "*/+-$)"};


int analysis(void);

int main()
{
    //�㷨4.2 ����Ԥ�������
    // ����G��ÿ������ʽ
    for (auto itG = G.begin(), itFirst = First.begin(); itG != G.end() && itFirst != First.end(); ++itG, ++itFirst)
    {
        // ���ս���±�ת��
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
    // ���Ԥ�������
    cout << "Ԥ�������" << endl;
    // ����ս��
    for (string::size_type i = 0; i < terminal.size(); ++i)
    {
        cout << '\t' << terminal[i];
    }
    cout << endl;
    // ������ս��
    for (string::size_type x = 0; x < nonTerminal.size(); ++x)
    {
        cout << nonTerminal[x];
        // �������ʽ
        for (string::size_type y = 0; y < table.at(x).size(); ++y)
            cout << '\t' << table.at(x).at(y);
        cout << endl;
    }
    cout << endl;
    return analysis();
    return 0;
}

// Ԥ���������
int analysis(void)
{
    cout << "���������������" << endl;
    //���뻺����
    string s;
    cin >> s;
    s.push_back('$');
    //����ջ
    vector<char> analyStack;
    analyStack.push_back('$');
    analyStack.push_back('E');
    // ջ���͵�ǰ����
    char top = '\0', cur = '\0';
    auto ip = s.begin();
    cout << endl << left << setw(10) << "ջ" << right << setw(20) << "����" << "\t" << "���" << endl;
    do
    {
        // �����ǰջ�͵�ǰʣ������
        string str1(analyStack.begin(), analyStack.end());
        string str2(ip, s.end());
        cout << left << setw(10) << str1 << right << setw(20) << str2 << "\t";
        // ջ���͵�ǰ�������
        top = analyStack.back();
        cur = *ip;
        // ��ʶ�������ֱ任
        if (isalpha(cur))
            cur = 'i';
        else if (isdigit(cur))
            cur = 'n';
        // ջ�����ս���Ż�$
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
                return -1;
            }
        }
        // ջ�����ս��
        else
        {
            int x = index.at(top);
            int y;
            try
            {
                y = index.at(cur);
            }
            catch (out_of_range)
            {
                cout << "�����ַ��Ƿ���" << endl;
                return -2;
            }
            // ����ʽ
            string production = table.at(x).at(y);
            // ����ʽ�ǿ�
            if (!production.empty())
            {
                analyStack.pop_back();
                string expr(production.begin() + 3, production.end());
                if (expr == "e")    // e����ʽ
                    expr = "";
                // ����ѹջ
                for (auto iter = expr.rbegin(); iter != expr.rend(); ++iter)
                    analyStack.push_back(*iter);
                // �������ʽ
                cout << production << endl;
            }
            else
            {
                cout << "error" << endl;
                return -1;
            }
        }
    }
    while (top != '$');
    return 0;
}
