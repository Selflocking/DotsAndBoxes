#include "board.h"
#include "define.h"
#include "fun.h"
#include "stupidAI.h"
Board brd;

// 其中"name?","new","move","end","quit","error"为平台向引擎传递命令；
// "name","move"为引擎向平台传递命令字
int main()
{
    string message;
    int ai;
    while (1)
    {
        cin >> message;
        if (message == "move")
        {
            int n;
            cin >> n >> message;
            //解析发来的信息
            for (int i = 0; i < message.size(); i += 3)
            {
                string t;
                t += message[i];
                t += message[i + 1];
                t += message[i + 2];
                //占线
                brd.occLine(ai, change(t));
            }
            //调用AI
            stupidAI(brd, ai);
            // randAI(brd,ai);
        }
        else if (message == "name?")
        {
            cout << "name stupidAI" << endl;
        }
        else if (message == "new")
        {
            cin >> message;
            // brd.reset();
            if (message == "black")
            {
                ai = BLACK;
                // stupidAi(ai);
                randAI(brd, ai);
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
            cin >> message;
            fflush(stdin);
            if (message == "black")
            {
                if (ai == BLACK) { cout << "Win!" << endl; }
                else
                {
                    cout << "Defect!" << endl;
                }
            }
            else
            {
                if (ai == WHITE) { cout << "Win!" << endl; }
                else
                {
                    cout << "Defect!" << endl;
                }
            }
        }
        else if (message == "quit")
        {
            cout << "Bye Bye" << endl;
            return 0;
        }
    }
    return 0;
}