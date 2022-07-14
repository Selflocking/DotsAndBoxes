//
// Created by YunShu on 2022/7/12.
//

#include "Node.h"
#include <cmath>
Node::Node()
{
    parent = nullptr;
    win = 0;
    total = 0;
    action = {-1, -1};
}
Node::Node(const Board &brd, int own)
{
    board = brd;
    owner = own;
    value = INF;
    parent = nullptr;
}
Node::Node(const Board &brd, int own, Node *prt, int val)
{
    board = brd;
    owner = own;
    parent = prt;
    value = val;
}
bool Node::operator<(const Node &other) const
{
    return this->value < other.value;
}
Node::Node(const Board &brd, int own, Node *prt, int val, LOC l)
{
    board = brd;
    owner = own;
    parent = prt;
    value = val;
    action = l;
    win = total = 0;
    board.occLine(owner, l);
}
bool Node::operator>(const Node &other) const
{
    return this->value > other.value;
}
void Node::UCB(int n)
{
    value = (double)win / total + C * sqrt(2*log(n) / total);
}
