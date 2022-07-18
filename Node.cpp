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
    UCBValue = (double)income / total + C * sqrt(log(n) / total);
}
