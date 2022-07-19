#include "define.h"
#include <functional>
#pragma once
class Board
{
  public:
    int map[11][11]{}; /**<棋盘*/
    int blackBox;      /**<黑放所占格子*/
    int whiteBox;      /**<白方所占格子*/
    Board();
    int getFreedom(int x, int y);                                  /**<得到自由度*/
    int getFreedom(LOC l);                                         /**<得到自由度*/
    int move(int owner, LOC l);                                    /**<下棋*/
    bool ifEarnBox(LOC l);                                         /**<是否赢得格子*/
    LOC eatCBox(int owner);                                        /**<吃C型格*/
    bool isFreeLine(LOC l) const;                                  /**<是否是自由边*/
    bool isFreeLine(int i, int j) const;                           /**<是否是自由边*/
    bool ifMakeCBox(LOC l);                                        /**<是否制造了C型格*/
    int getWinner() const;                                         /**<返回赢家*/
    bool ifEnd() const;                                            /**<判断游戏是否结束*/
    void print();                                                  /**<打印当前棋盘*/
    void traverseEdge(std::function<void(int i, int j)> const &f); /**<遍历所有的空边*/
    void traverseEdge(std::function<void(LOC l)> const &f);        /**<遍历所有的空边*/
};