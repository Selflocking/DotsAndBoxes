//
// Created by YunShu on 2022/7/12.
//

#include "Node.h"
Node::Node()
{
}
Node::Node(const Board &brd, int own)
{
    board = brd;
    owner = own;
    value = INF;
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
}
bool Node::operator>(const Node &other) const
{
    return this->value > other.value;
}
