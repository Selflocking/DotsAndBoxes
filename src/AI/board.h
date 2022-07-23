#include "define.h"
#include <functional>
#pragma once
class Board
{
  public:
    int map[11][11]{}; /**<棋盘*/
    int blackBox = 0;  /**<黑放所占格子*/
    int whiteBox = 0;  /**<白方所占格子*/
    int turn = 0;      /**<当前轮数*/
    Board();
    int getFreedom(int x, int y);                                        /**<得到自由度*/
    int getFreedom(LOC l);                                               /**<得到自由度*/
    int move(int player, LOC l);                                         /**<下棋*/
    void unmove(LOC l);                                                  /**<模拟时悔棋*/
    bool ifEarnBox(LOC l);                                               /**<是否赢得格子*/
    bool ifHaveSafeEdge();
    LOC eatCBox(int player);                                             /**<吃C型格*/
    bool isFreeLine(LOC l) const;                                        /**<是否是自由边*/
    bool isFreeLine(int i, int j) const;                                 /**<是否是自由边*/
    bool ifMakeCBox(LOC l);                                              /**<是否制造了C型格*/
    int getWinner() const;                                               /**<返回赢家*/
    bool ifEnd() const;                                                  /**<判断游戏是否结束*/
    LOC getPublicSide(LOC a,LOC b);      //获得两个格子的公共边，传入的坐标为(5*5)
    void print();                                                        /**<打印当前棋盘*/
    void traverseEdge(std::function<void(int i, int j)> const &f) const; /**<遍历所有的空边*/
    void traverseEdge(std::function<void(LOC l)> const &f) const;        /**<遍历所有的空边*/
};