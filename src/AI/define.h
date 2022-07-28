/*
 *此头文件是存放一些常用的宏定义
 */
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>
#include <utility>
#include <vector>

#pragma once

//各种链的编号  未定义，长链，短链，环，预备环，死链，死环
enum ChainType
{
    NotDefine,
    LongChain,
    ShortChain,
    Circle,
    PreCircle,
    DeadChain,
    DeadCircle
};

// 点
#define DOT 10

// 线，HENG是横线，SHU是竖线
//  OCCLINE代表occupied line，代表被占据的线
#define HENG 20
#define SHU 30
#define OCCLINE 40

// 格子，大于isBOX代表是格子
#define BOX 0

// 为了和格子对应
#define EMPTY 0
#define BLACK 1
#define WHITE -1

constexpr double C = 1; // UCB 常数
//定义格子类型
#define FULLBOX 0  //满格
#define DEADBOX 1  //死格
#define CHAINBOX 2 //链格
#define FREEBOX 3  //自由格
#define LEN 11     //棋盘数组长度
#define BOXLEN 5   //格子数组长度
#define BOXNUM 25

// UCT
#define UCT_MC_TIMES 1
#define UCT_TIMES 15000000
#define UCT_LIMIT_TIME 32
#define UCT_FILTER_RANGE 30

// 一些常用的
// 不适用using namespace std,是为了防止和std空间里的命名冲突
using LOC = std::pair<int, int>;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::string;
using std::vector;