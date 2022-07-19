#include "Node.h"
#include "board.h"
#include "define.h"
#include "fun.h"
#include "UCT.h"

Board * brd;

// 其中"name?","new","move","end","quit","error"为平台向引擎传递命令；
// "name","move"为引擎向平台传递命令字

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
            UCT(brd, ai);
        }
        else if (message == "name?")
        {
            cout << "name UCT" << endl;
        }
        else if (message == "new")
        {
            cin >> message;
            brd = new Board;
            if (message == "black")
            {
                ai = BLACK;
                UCT(brd, ai);
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