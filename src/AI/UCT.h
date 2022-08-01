#include "Node.h"
#include "board.h"
#include <mutex>
#include <random>
#include <thread>


#ifndef DOTS_AND_BOXES_UCT_H
#define DOTS_AND_BOXES_UCT_H

constexpr int TIME = 30;   // 时间限制
constexpr int THREAD = 12; // 线程最大限制
constexpr int SIM = 1;     // 模拟次数

using std::mutex;
using std::ref;
using std::thread;

int getBoardWinner(Board &CB, int LatterPlayer);             //*得到或者预测当前局面胜利者
int getBoardWinner(Board &CB, int LatterPlayer, int &score); //*得到或者预测当前局面胜利者

// FilterMC，
float getFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range);
//*在扩展子节点时，为子节点初次评估一个收益
int getFilterMCWinner(Board &CB, int NextPlayer, int Filter_Range);             //*返回模拟的胜负结果
int getFilterMCWinner(Board &CB, int NextPlayer, int Filter_Range, int &score); //*返回模拟的胜负结果
int rndFilterTurn(Board &CB, int Player, bool Msg, int Filter_Range);           //*用于模拟时随机占边

float UCTProcess(Node &B, int &Total, int MC_Times, int Filter_Range); //*uct搜索
void UCTMove(Board &CB, int Player, bool Msg, vector<LOC> &pace);      //*用UCT算法进行移动

void deleteUCTNode(Node *Root); //*用于释放空间
void deleteUCTTree(Node Root);  //*用于释放空间

void UCTMoveWithSacrifice(Board &CB, int Player, bool Msg, vector<LOC> &pace); //*包括UCT搜索前的预处理
void latterSituationMove(Board &CB, int Player, vector<LOC> &pace); //*基于特殊结构体的决策，一般用于后期
void gameTurnMove(Board &CB, int Player, bool Msg, int *status, vector<LOC> &pace); //*根据前后期自动移动
void multi_thread_func(Board &CB, int &a, int NextPlayer, int Winner, int Filter_Range);

#endif // DOTS_AND_BOXES_UCT_H