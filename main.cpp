#include "Node.h"
#include "board.h"
#include "define.h"
#include "fun.h"
#include <ctime>
#include <queue>
#include <random>
Board brd;
constexpr int LIMIT = 30;
constexpr int SEARCH = 1000;
LOC UCTSearch(Board board, int owner,int deep);
Node expend(Node *parent, Board, int owner, LOC);
double play(Node *node);
void UCT(Board &board, int owner);
// 其中"name?","new","move","end","quit","error"为平台向引擎传递命令；
// "name","move"为引擎向平台传递命令字
// 我们需要记录棋谱因此需要两个本地txt文件名字规定为chess.txt
int main()
{
    string message;
    int ai;
    fstream log1;
    log1.open("chess.txt", ios::out);
    while (true)
    {
        cin >> message;
        log1 << message << endl;
        if (message == "move")
        {
            int n;
            cin >> n >> message;
            log1 << n << " " << message << endl;
            // 解析并log（录入日志）发来的信息
            for (int i = 0; i < message.size(); i += 3)
            {
                string t;
                t += message[i];
                t += message[i + 1];
                t += message[i + 2];
                // 占线
                brd.occLine(-ai, change(t));
            }
            UCT(brd, ai);
        }
        else if (message == "name?")
        {
            cout << "name stupidAI" << endl;
            log1 << "name stupidAI" << endl;
        }
        else if (message == "new")
        {
            cin >> message;
            log1 << message << endl;
            // brd.reset();
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
            log1 << "error! check it!" << endl;
        }
        else if (message == "end")
        {
            cin >> message;
            log1 << message << endl;
            fflush(stdin);
            if (message == "black")
            {
                if (ai == BLACK)
                {
                    cout << "Win!" << endl;
                }
                else
                {
                    cout << "Defect!" << endl;
                }
            }
            else
            {
                if (ai == WHITE)
                {
                    cout << "Win!" << endl;
                }
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
}

LOC UCTSearch(Board board, int owner,int deep)
{
    cout << "UCTSearch Begin\n";
    Node root(board, owner);
    std::priority_queue<Node, vector<Node>, std::greater<>> heap;
    board.print();
    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            if (board.isFreeLine({i, j}))
            {
                heap.push(expend(&root, board, -owner, LOC(i, j)));
            }
        }
    }
    time_t begin = time(nullptr);
    int n = 0;
    while (time(nullptr) - begin < LIMIT)
    {
        Node t = heap.top();
        heap.pop();
        ++n;
        t.value = play(&t) + sqrt(2 * log(n * SEARCH) / SEARCH);
        heap.push(t);
    }
    cout<<"Max value: "<<heap.top().value<<"\n";
    return heap.top().action;
}
double play(Node *node)
{
    int win = 0;
    int cnt = -1;
    vector<LOC> lines;
    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            if (node->board.isFreeLine({i, j}))
            {
                lines.emplace_back(LOC{i, j});
            }
        }
    }
    Node t = *node;
    vector<LOC> lt = lines;
    while (++cnt < SEARCH)
    {
        srand(time(nullptr));
        std::shuffle(lt.begin(), lt.end(), std::mt19937(std::random_device()()));
        while (!lt.empty())
        {
            while (!lt.empty() && t.board.occLine(t.owner, lt.back()))
            {
                lt.pop_back();
            }
            if (!lt.empty())
            {
                lt.pop_back();
            }
            t.owner = -t.owner;
        }

        if (t.board.winner() == node->owner)
            ++win;
        t = *node;
        lt = lines;
    }
    return 1.0 * win / SEARCH;
}

Node expend(Node *parent, Board board, int owner, LOC l)
{
    board.occLine(owner, l);
    parent->n++;
    return Node{board, owner, parent, INF, l};
}
void UCT(Board &board, int owner)
{
    string res;
    LOC l = UCTSearch(board, owner, 1);
    res += change(l);
    while (board.occLine(owner, l))
    {
        LOC t = UCTSearch(board, owner,1);
        res += change(t);
    }
    cout << "move " << res.size() / 3 << " " << res << endl;
}