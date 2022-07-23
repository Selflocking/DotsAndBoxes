#include "board.h"
#include "define.h"
#ifndef DOTS_AND_BOXS_NODE_H
#define DOTS_AND_BOXS_NODE_H

constexpr int INF = 1e9; /**< 无穷大 */

class Node
{
  public:
    Board board;                        /**< 棋面 */
    Node *parent = nullptr;                       /**< 父节点 */
    double income = 0;                  /**< 收益值 */
    int total = 0;                      /**< 总共模拟次数*/
    double UCBValue = INF;                /**< UCB值 */
    int player;                         /**< 节点所属下棋方 */
    vector<LOC> moves;                   /**< 可行的落子位置 */
    LOC action;                         /**< 节点代表的动作 */
    vector<Node* > children;              /**< 节点的子节点 */
    Node(Board &board, int player);
    Node(Board &board, int player, Node *parent, LOC action); /**< 构造函数 */
    bool operator<(const Node &) const; /**< @brief 排序需要*/
    bool operator>(const Node &) const; /**< @brief 排序需要*/
    void computeUCB(int n);             /**< @brief 计算该节点UCB值*/
    void expand();                     /**< @brief 展开节点*/
};

#endif // DOTS_AND_BOXS_NODE_H
