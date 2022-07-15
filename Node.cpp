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
Node::Node(const Board &brd, int own, Node *prt, LOC l)
{
    board = brd;
    owner = own;
    parent = prt;
    action.emplace_back(l);
    value = win = total = 0;
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
Node::Node(const Board &board, int owner, Node *parent)
{
    this->board = board;
    this->owner = owner;
    this->parent = parent;
    value = win = total = 0;
}
Node::Node(const Board &board, int owner, Node *parent, const vector<LOC> &action)
{
    this->board = board;
    this->owner = owner;
    this->parent = parent;
    value = win = total = 0;
    this->action = action;
}
Node::Node(const Node &other, LOC action)
{
    this->value = other.value;
    this->owner = other.owner;
    this->parent = other.parent;
    this->board = other.board;
    this->total = other.total;
    this->win = other.win;
    this->action = other.action;
    this->action.emplace_back(action);
    this->board.occLine(other.parent->owner,action);
}
