#include "UCT.h"
#include "Node.h"
#include "board.h"
#include "define.h"
#include <random>
#include <thread>
Node *root;
time_t begin;
void UCT(Board *board, int player)
{
    system("pause");
    string res;        // 引擎需要
    vector<LOC> moves; // 可行的位置
    // moves.emplace_back(LOC{0,1});
   UCTSearch(*board, player, moves);
    if (moves.empty())
    {
        cout << "Game Over" << endl;
    }
    else
    {
        for (auto &i : moves)
        {
            board->move(player, i);
        }
    }
    board->turn++;
}
void UCTSearch(Board board, int player, vector<LOC> &res)
{
    root = new Node(board, player);
    begin = time(nullptr);
    preProcess(root);
    while (time(nullptr) - begin < TIME)
    {
        if (TreePolicy(root))
        {
            break;
        }
    }
}
bool TreePolicy(Node *node)
{
    while (!node->board.ifEnd())
    {
        if (node->children.empty())
        {
            if (node->board.ifHaveSafeEdge())
            {
                preProcess(node);
                node->expand();
            }
            else
            {
                preProcess2(node);
            }
        }
        else
        {
            Node *mx = *max_element(node->children.begin(), node->children.end());
            if (mx->total == 0)
            {
                Simulation(mx);
                return false;
            }
            else
            {
                node = mx;
            }
        }
    }
    return true;
}
void preProcess(Node *node)
{
    LOC temp;
    while (true)
    {
        temp = node->board.eatCBox(node->player);
        if (temp.first == -1)
            break;
        node->moves.emplace_back(temp);
    }
}
void preProcess2(Node *node)
{
    /*
     * 假设终局有一个长链，在长链的中间插入边会形成两个长C
     */
    /*
     * 长链即使不是最后一个也可以吃完
     */
    /* 预处理1
     * 双交不得不吃
     * 两格短C可以选择不吃
     * 三个短C可以吃完也可以留双交
     * 四格及以上长C除非下完棋局结束否则留双交
     * 四个死环要么占满，要么留两个双交
     * 六格死环应该是可以吃完或者留两个双交？
     * 八格及以上死环除非下完棋局结束否则留两个双交
     *
     * 当然，也有可能对方比较傻，提前开长链，暂不考虑
     */
    /*预处理2:
     * 如果我们是先手并且场上目前没有两格短链，那么我们应该先开环，再开长链，
     * 因为开环，对手如果想保持控制权，必须给我们留两个双交，而开长链，对手只能留双交
     */

    /* 拓展节点：
     * 两格短链和双交是交换先后手的关键
     */
    /*拓展节点：
     * 对于链与环相交，必须下链中的边，这样可以将链和环分割开来，因为如果我们在环中下边，这个特殊结构会转变成死树，利好对手
     *
     * 对于环与环相交，每个环里的边为等价边，如果两个环相同，则整个结构中所有边为等价边
     *
     * 对于链与链相交，每个链里的边为等价边，如果组成的链长度相同，则整个结构中所有边为等价边
     */

    /*特殊结构判断
     * 简单的特殊结构为长链和环，
     * 长链的每一个格子自由度都为2，但是头尾不是同一个格子
     * 环的每一个格子自由度都为2，但是头尾是同一个格子
     *
     * 复杂的特殊结构为链与环相交（包括一格的链与环相交），链与链相交，环与环相交
     * 交叉处的格子自由度等于周围链格和环格的数目和
     */
    /*特殊结构判断：
     * 对于更为复杂的情况，比如一条链既与链相交，又与环相交，
     * 还是能分割成更小的部分进行处理
     * 这就要求，存储特殊结构时，并不是将诸如链与链这样的特殊结构整个存储，而是存下单独的链和环，使用相交点来判断是否为复杂的特殊结构
     */
}
void Simulation(Node *node)
{
}
void backup(Node *node, int win)
{
    int bk = node->player;
    while (node != nullptr)
    {
        node->total += SIM;
        if (node->player == bk)
        {
            node->income += SIM - win;
        }
        else
        {
            node->income += win;
        }
        node = node->parent;
    }
}