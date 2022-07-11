#include "define.h"

#pragma once
class Board
{
  public:
    int8_t map[11][11]{};        // 使用int8节省空间
    vector<LOC> freeline;      // getRandLine()使用，用到时才创建
    vector<vector<LOC>> chain; // 存放长链
    Board();                   // 默认初始化棋盘
    Board(Board const &other);
    Board(int8_t m[11][11]);            // 从另一个棋盘初始化
    int getFreedom(int x, int y);       // 得到自由度
    int getFreedom(LOC l);              // 得到自由度
    void printBoard();                  // 打印棋盘
    int occLine(int owner, LOC l);      // 占线 occupy Line
    LOC eatCBoxs(int owner);            // 吃C型格
    vector<LOC> eatAllCBoxs(int owner); // 吃完棋盘上的全部C型格
    bool isFree(LOC l);                 // 检查一条边是否被占
    LOC getRandLine();                  // 得到随机边
    bool makeCBox(LOC l);               //判断一条线能否形成C型格
    void reset();                       // 重置棋盘
};

// 判断奇偶函数，暂时没用
//  inline bool isEven(int x){
//      return x&1;
//  }
//  inline bool isOdd(int x){
//      return !isEven(x);
//  }