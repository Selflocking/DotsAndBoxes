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

constexpr int TIME = 30;   // 时间限制
constexpr int EXPAND = 1000; //拓展次数限制
constexpr int PLAY = 100;  // 演绎次数
constexpr int THREAD = 12; // 线程最大限制

vector<LOC> *UCTSearch(Board *board, int owner);
bool TreePolicy(Node *node);
double UCB(int win, int total, int n);
void play(Node *node);
void BackUp(Node *node, int win, int total);
void UCT(Board *board, int owner);
void Expand(Node *node);
void ExpandEx(Node *node);
#endif // DOTS_AND_BOXS_UCT_H
