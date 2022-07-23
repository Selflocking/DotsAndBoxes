#include "Node.h"
#include "board.h"
#include "define.h"
#include <random>
#include <thread>

#ifndef DOTS_AND_BOXS_UCT_H
#define DOTS_AND_BOXS_UCT_H

constexpr int TIME = 30;   // 时间限制
constexpr int THREAD = 12; // 线程最大限制
constexpr int SIM = 1;    // 模拟次数

void UCT(Board *board, int player); /**< UCT入口函数，使用*是为了改变board的值 */
void UCTSearch(Board board, int player, vector<LOC> &res);
bool TreePolicy(Node *node);
void preProcess(Node *node);
void preProcess2(Node *node);
void Simulation(Node *node);
void backup(Node *node, int win);
#endif
