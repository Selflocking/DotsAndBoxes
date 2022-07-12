#include "define.h"

#pragma once
class Board
{
  public:
    int map[11][11]; // 使用int8节省空间
    Board();         // 默认初始化棋盘
    Board(Board const &other);
    Board(int m[11][11]);               // 从另一个棋盘初始化
    int getFreedom(int x, int y);       // 得到自由度
    int getFreedom(LOC l);              // 得到自由度
    void printBoard();                  // 打印棋盘
    int occLine(int owner, LOC l);      // 占线 occupy Line
    LOC eatCBoxs(int owner);            // 吃C型格
    vector<LOC> eatAllCBoxs(int owner); // 吃完棋盘上的全部C型格
    bool isFreeLine(LOC l) const;       // 检查一条边是否被占
    bool makeCBox(LOC l);               // 判断一条线能否形成C型格
    void reset();                       // 重置棋盘
    int winner() const;
    Board &operator=(const Board &other);
    void print();
};

// 判断奇偶函数，暂时没用
//  inline bool isEven(int x){
//      return x&1;
//  }
//  inline bool isOdd(int x){
//      return !isEven(x);
//  }