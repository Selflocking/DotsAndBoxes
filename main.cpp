#include "Node.h"
#include "board.h"
#include "define.h"
#include "fun.h"
#include <ctime>
#include <pthread.h>
#include <queue>
#include <random>
#include <thread>
Board brd;
constexpr int LIMIT = 30;
constexpr int SEARCH = 100;
constexpr double DEEP = 3;
constexpr int THREAD = 12;
LOC UCTSearch(Board board, int owner);
void NodeSearch(Node &node, int deep);
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
        }
        else if (message == "quit")
        {
            cout << "Bye Bye" << endl;
            return 0;
        }
    }
}

LOC UCTSearch(Board board, int owner)
{
    Node root(board, owner);
    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            if (board.isFreeLine({i, j}))
            {
                root.children.emplace_back(Node{board, -owner, &root, INF, LOC{i, j}});
            }
        }
    }
    int max = 0;
    LOC res(-1, -1);
    for (int i = 0; i < root.children.size() / THREAD; ++i)
    {
        std::thread t[THREAD];
        for (int j = i * THREAD; j < i * THREAD + THREAD && j < root.children.size(); ++j)
        {
            t[j%THREAD] = std::thread(NodeSearch, std::ref(root.children[j]), 1);
        }
        for (int j = i * THREAD; j < i * THREAD + THREAD && j < root.children.size(); ++j)
        {
            t[j%THREAD].join();
        }
    }
    //    for (int i = 0; i < root.children.size(); ++i)
    //    {
    //        t[i] = std::thread(NodeSearch, std::ref(root.children[i]), 1);
    //    }
    //    for (int i = 0; i < root.children.size(); ++i)
    //    {
    //        t[i].join();
    //    }
    for (auto &i : root.children)
    {
        if (i.win >= max)
        {
            max = i.win;
            res = i.action;
        }
    }
    return res;
}
void NodeSearch(Node &node, int deep)
{
    if (deep >= DEEP)
    {
        play(&node);
        return;
    }
    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            if (node.board.isFreeLine({i, j}))
            {
                node.children.emplace_back(Node{node.board, -node.owner, &node, INF, LOC{i, j}});
            }
        }
    }
    for (auto &i : node.children)
    {
        NodeSearch(i, deep + 1);
        node.win += i.win;
        node.n += i.n;
    }
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
        {
            ++win;
        }
        t = *node;
        lt = lines;
    }
    node->parent->n += SEARCH;
    node->parent->win += win;
    return 1.0 * win / SEARCH;
}

void UCT(Board &board, int owner)
{
    time_t begin = time(nullptr);

    string res;
    LOC l;
    do
    {
        l = UCTSearch(board, owner);
        if (l.first == -1)
            return;
        res += change(l);
    } while (board.occLine(owner, l));

    time_t cost = time(nullptr) - begin;
    if (cost > 60)
    {
        printf("cost: %2d min %2d sec\n", cost / 60, cost % 60);
    }
    else
    {
        printf("cost: %2d sec\n", cost);
    }
    cout << "move " << res.size() / 3 << " " << res << endl;
}