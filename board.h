#include "define.h"

#pragma once
class Board
{
  public:
    int map[11][11]{};                                               /**< 棋盘 */
    int blackBox;                                                  /**< 黑方格子数 */
    int whiteBox;                                                  /**< 白方格子数 */
    Board();                                                       /**< 默认初始化棋盘 */
    Board(Board const &other);                                     /**< 从另一个棋盘初始化 */
    Board(const Board & other, const int &owner, const LOC&  action);
    int getFreedom(int x, int y);                                  /**< 得到自由度 */
    int getFreedom(LOC l);                                         /**< 得到自由度*/
    void printBoard();                                             /**< 打印棋盘*/
    int occLine(int owner, LOC l);                                 /**< 占线 occupy Line*/
    bool earnBox(LOC l);
    LOC eatCBoxs(int owner);                                       /**< 吃C型格*/
    bool isFreeLine(LOC l) const;                                  /**< 检查一条边是否被占*/
    bool isFreeLine(int i, int j) const;                           /**< 检查一条边是否被占*/
    bool makeCBox(LOC l);                                          /**< 判断一条线能否形成C型格*/
    int winner() const;                                            /**< 判断赢家*/
    Board &operator=(const Board &other);                          /**< 重载等于号*/
    void print();                                                  /**< 打印当前棋局*/
    void traverseEdge(std::function<void(int i, int j)> const &f); /**< 对所有边进行某种操作*/
    void traverseEdge(std::function<void(LOC l)> const &f); /**< 对所有边进行某种操作*/
};

// 判断奇偶函数，暂时没用
//  inline bool isEven(int x){
//      return x&1;
//  }
//  inline bool isOdd(int x){
//      return !isEven(x);
//  }