#include "board.h"
#include "define.h"
#include "fun.h"
#include "stupidAI.h"
Board brd;

// 其中"name?","new","move","end","quit","error"为平台向引擎传递命令；
// "name","move"为引擎向平台传递命令字
// 我们需要记录棋谱因此需要两个本地txt文件名字规定为chess.txt
int main()
{
    string message;
    int ai;
    fstream log1;  
    log1.open("chess.txt",ios::out);
    while (true)
    {
        cin >> message;
        log1 << message << endl;
        if (message == "move")
        {
            int n;
            cin >> n >> message;
            log1<<n<<" "<<message<<endl;
            //解析并log（录入日志）发来的信息
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
            //stupidAI(brd, ai,log1);
            randAI(brd,ai,log1);
        }
        else if (message == "name?")
        {
            cout << "name stupidAI" << endl;
            log1 << "name stupidAI" << endl;
        }
        else if (message == "new")
        {
            cin >> message;
            log1 << message <<endl; 
            // brd.reset();
            if (message == "black")
            {
                ai = BLACK;
                //stupidAi(ai);
                randAI(brd, ai,log1);
            }
            else
            {
                ai = WHITE;
            }
        }
        else if (message == "error")
        {
            cout << "error! check it!" << endl;
            log1 << "error! check it!" << endl;
        }
        else if (message == "end")
        {
            cin >> message;
            log1<< message << endl;
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