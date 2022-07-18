#include "board.h"
#include "define.h"
#ifndef DOTS_AND_BOXS_NODE_H
#define DOTS_AND_BOXS_NODE_H

constexpr int INF = 0x7FFFFFFF; // int的最大值，用来当作无穷大

class Node
{
  public:
    Board board;                                             /**< 棋面 */
    Node *parent;                                            /**< 父节点 */
    double win = 0;                                             /**< 胜利次数 */
    int total = 0;                                           /**< 总共模拟次数*/
    double value = 0;                                            /**< UCB值 */
    int owner;                                               /**< 节点所属下棋方 */
    vector<LOC> action;                                      /**< 节点代表的动作 */
    vector<Node> children;                                   /**< 节点的子节点 */
    Node();                                                  /**< @brief 默认构造函数，用于创建根节点*/
    Node(const Board &, int owner);                          /**< @brief 从一个棋面创建节点*/
    Node(const Board &, int owner, Node *parent, int value); /**< @brief 拓展需要*/
    Node(const Board &, int owner, Node *parent, LOC l);     /**< @brief 拓展子节点需要*/
    Node(const Board & board,int owner,Node *parent);
    Node(const Board &board, int owner, Node *parent, const vector<LOC> &action);
    Node(const Node& other,LOC action);
    bool operator<(const Node &) const; /**< @brief 排序需要*/
    bool operator>(const Node &) const; /**< @brief 排序需要*/
    void UCB(int n);
};

#endif // DOTS_AND_BOXS_NODE_H
