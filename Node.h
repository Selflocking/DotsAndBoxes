//
// Created by YunShu on 2022/7/12.
//
#include "board.h"
#include "define.h"
#ifndef DOTS_AND_BOXS_NODE_H
#define DOTS_AND_BOXS_NODE_H
constexpr int INF = 0x7FFFFFFF;
class Node
{
  public:
    Board board;
    Node *parent;
    int n;
    double value;
    int owner;
    LOC action;
    Node();
    Node(const Board &, int);
    Node(const Board &, int, Node *, int);
    Node(const Board &, int, Node *, int, LOC);
    bool operator<(const Node &) const;
    bool operator>(const Node &) const;
};

#endif // DOTS_AND_BOXS_NODE_H
