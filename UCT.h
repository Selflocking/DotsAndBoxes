//
// Created by YunShu on 2022/7/13.
//
#include "Node.h"
#include "board.h"
#include "define.h"
#include <random>
#include <thread>

#ifndef DOTS_AND_BOXS_UCT_H
#define DOTS_AND_BOXS_UCT_H

LOC UCTSearch(Board board, int owner); // UCT开始搜索函数
void NodeSearch(Node &node, int deep); // 递归搜索函数，deep代表搜索深度
double play(Node *node);               // 随机演绎函数
void UCT(Board &board, int owner);     // UCT下棋函数

#endif // DOTS_AND_BOXS_UCT_H
