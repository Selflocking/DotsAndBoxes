#include "Node.h"
#include <cmath>
bool Node::operator<(const Node &other) const
{
    return this->UCBValue < other.UCBValue;
}
bool Node::operator>(const Node &other) const
{
    return this->UCBValue > other.UCBValue;
}
void Node::computeUCB(int n)
{
    if(total==0) return ;
    UCBValue = (double)income / total + C * sqrt(2*log(n) / total);
}
void Node::expand()
{
    board.traverseEdge([this](LOC l){
        this->children.emplace_back(new Node(board,-player,this,l));
    });
}
Node::Node(Board &board, int player, Node *parent, LOC action)
{
    this->board = board;
    this->player = player;
    this->parent = parent;
    this->action = action;
    board.move(parent->player,action);
}
Node::Node(Board &board, int player)
{
    this->board = board;
    this->player = player;
}
