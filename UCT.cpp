//
// Created by YunShu on 2022/7/13.
//
#include "Node.h"
#include "board.h"
#include "define.h"
#include "fun.h"
#include <random>
#include <thread>

/**
 * @brief 随机演绎函数
 * @param node 表示开始演绎的节点
 * @return 返回胜率
 * @todo 目前的演绎强制演绎一定次数，需要修正，避免重复演绎和无效演绎
 */
double play(Node *node)
{
    int win = 0;       // 统计胜利次数
    int cnt = -1;      // 统计演绎次数
    vector<LOC> lines; // 存放目前节点所有空边
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

    Node t = *node;         // 复制节点
    vector<LOC> lt = lines; // 复制空边
    while (++cnt < SEARCH)  // 开始演绎
    {
        // 置乱边集
        srand(time(nullptr));
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

        // 重新复制节点和边集
        t = *node;
        lt = lines;
    }

    // 回溯，将当前胜利次数和演绎总数传递给父节点
    node->parent->n += SEARCH;
    node->parent->win += win;
    return 1.0 * win / SEARCH;
}

/**
 * @brief 递归拓展节点，利用宏DEEP控制拓展最大深度
 * @param node 需要被拓展的节点
 * @param deep 目前所在深度
 */
void NodeSearch(Node &node, int deep)
{
    // 如果达到搜索深度，开始演绎
    if (deep >= DEEP)
    {
        play(&node);
        return;
    }

    // 如果没有达到搜索深度，继续拓展节点
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

    // 开始递归，同时回溯，将当前节点胜利次数和演绎总数传递给父节点
    for (auto &i : node.children)
    {
        NodeSearch(i, deep + 1);
        node.win += i.win;
        node.n += i.n;
    }
}

/**
 * @brief UCT搜索函数，做一些搜索的开始工作，调用NodeSearch()开始递归拓展
 * @param board 搜索开始棋面
 * @param owner 下棋方
 * @return UCT认为最佳的着棋点
 * @warning 目前并没有完全实现UCT，仅仅使用演绎胜率作为参考
 */
LOC UCTSearch(Board board, int owner)
{
    Node root(board, owner); // 使用当前局面建立根节点
    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            if (board.isFreeLine({i, j})) // 寻找空边，建立子节点
            {
                root.children.emplace_back(Node{board, -owner, &root, INF, LOC{i, j}});
            }
        }
    }

    int max = 0;
    LOC res(-1, -1);

    // 多线程，利用宏THREAD控制最大线程数
    // 递归拓展子节点
    for (int i = 0; i < root.children.size() / THREAD; ++i)
    {
        std::thread t[THREAD];
        for (int j = i * THREAD; j < i * THREAD + THREAD && j < root.children.size(); ++j)
        {
            t[j % THREAD] = std::thread(NodeSearch, std::ref(root.children[j]), 1);
        }
        for (int j = i * THREAD; j < i * THREAD + THREAD && j < root.children.size(); ++j)
        {
            t[j % THREAD].join();
        }
    }

    // 遍历所以子节点，选取最优子节点
    for (const auto &i : root.children)
    {
        if (i.win >= max)
        {
            max = i.win;
            res = i.action;
        }
    }
    return res;
}

/**
 * @brief UCT开始函数，调用UCTSearch()
 * @param board 目前棋面
 * @param owner 下棋方
 */
void UCT(Board &board, int owner)
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
        board.occLine(owner, l)); // 占领这条线，occline()返回这条线占领的格子的数量，如果占领了某格子，需要接着搜索

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