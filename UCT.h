#include "Node.h"
#include "board.h"
#include "define.h"
#include <random>
#include <thread>

#ifndef DOTS_AND_BOXS_UCT_H
#define DOTS_AND_BOXS_UCT_H

constexpr int TIME = 30;     // 时间限制
constexpr int THREAD = 12;   // 线程最大限制

void UCT(Board *board, int owner); /**< UCT入口函数，使用*是为了改变board的值 */
#endif
