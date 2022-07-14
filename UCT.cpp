#include "UCT.h"
#include "Node.h"
#include "board.h"
#include "define.h"
#include "fun.h"
#include <random>
#include <thread>

Node *root;
LOC UCTSearch(Board *board, int owner)
{
    root = new Node(*board, owner);
    Node *t = root;
    time_t begin = time(nullptr);
    while (time(nullptr) - begin < TIME)
    {
        if (Expand(root))
            break;
    }

    double max = -1;
    LOC action = {-1, -1};
    if (root->children.empty())
        return {-1, -1};
    cout << root->children.size() << endl;
    for (auto &i : root->children)
    {
        i.UCB(root->total);
        if (i.value > max)
        {
            max = i.value;
            action = i.action;
        }
    }
    delete root;
    return action;
    // return this child that has max UCB value.
}

bool Expand(Node *node)
{
    while (node->board.winner() == EMPTY)
    {
        if (node->children.empty())
        {
            node->board.traverseEdge([&node](int i, int j) {
                node->children.emplace_back(Node{node->board, -node->owner, node, 0, LOC{i, j}});
            });
            std::thread t[node->children.size()];
            for (int i = 0; i < node->children.size(); ++i)
            {
                t[i] = std::thread(play, &node->children[i]);
            }
            for (int i = 0; i < node->children.size(); ++i)
            {
                t[i].join();
            }
            return false;
        }
        else
        {
            double max = -1;
            Node *next = &node->children[0];
            for (auto &i : node->children)
            {
                i.UCB(root->total);
                if (i.value > max)
                {
                    max = i.value;
                    next = &i;
                }
            }
            node = next;
        }
    }
    return true;
}

double UCB(int win, int total, int n)
{
    return (double)win / total + C * sqrt(2*log(n) / total);
}

void play(Node *node)
{
    int win = 0;
    int cnt = -1;
    vector<LOC> lines;
    node->board.traverseEdge([&lines](int i, int j) { lines.emplace_back(LOC{i, j}); });
    Node t = *node;
    vector<LOC> lt = lines;

    // 置乱边集
    std::shuffle(lt.begin(), lt.end(), std::mt19937(std::random_device()()));

    // 当没有下完时，轮流着棋
    while (!lt.empty())
    {
        // 当还可以着边，并且所着边占据了格子时，连续着边
        while (!lt.empty() && t.board.occLine(t.owner, lt.back()))
        {
            lt.pop_back();
        }
        // 弹出已经被下的边
        if (!lt.empty())
        {
            lt.pop_back();
        }
        // 交换下棋方
        t.owner = -t.owner;
    }

    // 计算胜利与否
    if (t.board.winner() == node->owner)
    {
        ++win;
    }

    node->win = win;
    node->total = PLAY;
    BackUp(node, win, PLAY);
}

void BackUp(Node *node, int win, int total)
{
    Node *t = node;
    while (node->parent != nullptr)
    {
        node = node->parent;
        node->total += total;

        if (node->owner == t->owner)
            node->win += win;
        else
            node->win += total - win;
    }
}
void UCT(Board *board, int owner)
{
    time_t begin = time(nullptr); // 记录开始时间

    string res; // 引擎需要
    LOC l;      // 多次下棋需要

    do
    {
        l = UCTSearch(board, owner); // 得到一个最佳着棋点
        if (l.first == -1)           // 如果棋面已经结束
            return;
        res += change(l); // 转换成协议可以理解的字符串
    } while (
        board->occLine(owner, l)); // 占领这条线，occline()返回这条线占领的格子的数量，如果占领了某格子，需要接着搜索

    time_t cost = time(nullptr) - begin; // 计算计算时间
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