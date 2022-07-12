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
    int win = 0;
    int n = 1;
    double value ;
    int owner;
    LOC action;
    vector<Node> children;
    Node();
    Node(const Board &, int owner);
    Node(const Board &, int owner, Node * parent, int value);
    Node(const Board &, int owner, Node * parent, int value, LOC l);
    bool operator<(const Node &) const;
    bool operator>(const Node &) const;
};

#endif // DOTS_AND_BOXS_NODE_H
