#include "AI/Node.h"
#include "AI/UCT.h"
#include "AI/board.h"
#include "AI/define.h"
#include <corecrt.h>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
Board *brd;
int temp = 0;
// 其中"name?","new","move","end","quit","error"为平台向引擎传递命令；
// "name","move"为引擎向平台传递命令字
string change(LOC l);
LOC change(string l);

int main()
{
    string message;
    int ai;
    while (true)
    {
        cin >> message;
        if (message == "move")
        {
            int n;
            cin >> n >> message;
            for (int i = 0; i < message.size(); i += 3)
            {
                string t;
                t += message[i];
                t += message[i + 1];
                t += message[i + 2];
                // 占线
                brd->move(-ai, change(t));
            }
            vector<LOC> steps;
            gameTurnMove(*brd, ai, false, &temp, steps);
            string res;
            for (auto const &i : steps)
            {
                res += change(i);
            }
             cout << "move " << res.size() / 3 << " " << res << endl;
        }
        else if (message == "name?")
        {
            cout << "name singleThread" << endl;
        }
        else if (message == "new")
        {
            cin >> message;
            brd = new Board;
            if (message == "black")
            {
                ai = BLACK;
                vector<LOC> steps;
                gameTurnMove(*brd, ai, false, &temp, steps);
                string res;
                for (auto const &i : steps)
                {
                    res += change(i);
                }
                 cout << "move " << res.size() / 3 << " " << res << endl;
            }
            else
            {
                ai = WHITE;
            }
        }
        else if (message == "error")
        {
            cout << "error! check it!" << endl;
        }
        else if (message == "end")
        {
            delete brd;
        }
        else if (message == "quit")
        {
            return 0;
        }
    }
}

string change(LOC l)
{
    string s;
    if (l.first % 2 == 0 && l.second % 2 == 1)
    {
        s += 'A';
        s += 'A' + l.first / 2;
        s += 'A' + l.second / 2;
    }
    else
    {
        s += 'B';
        s += 'A' + l.second / 2;
        s += 'A' + l.first / 2;
    }
    return s;
}

/**
 * @brief 将字符串转换成坐标
 * @param l 对战平台传进的坐标值
 * @return 坐标值
 */
LOC change(string l)
{
    LOC res;
    if (l[0] == 'A')
    {
        res.first = (l[1] - 'A') * 2;
        res.second = (l[2] - 'A') * 2 + 1;
    }
    else
    {
        res.second = (l[1] - 'A') * 2;
        res.first = (l[2] - 'A') * 2 + 1;
    }
    return res;
}