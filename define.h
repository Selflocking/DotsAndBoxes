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
// 点
#define DOT 10

// 线，HENG是横线，SHU是竖线
//  OCCLINE代表occupied line，代表被占据的线
#define HENG 20
#define SHU 30
#define OCCLINE 40

// 格子，大于isBOX代表是格子
#define BOX 0

//定义格子类型
#define FULLBOX	0		//满格
#define DEADBOX 1		//死格
#define CHAINBOX 2		//链格
#define FREEBOX 3		//自由格

// 为了和格子对应
#define EMPTY 0
#define BLACK 1
#define WHITE -1

#define LEN 11      //棋盘数组长度
#define BOXLEN 5	//格子数组长度
#define BOXNUM 25

constexpr int LIMIT = 30;   // 时间限制
constexpr int SEARCH = 100; // 搜索次数限制
constexpr double DEEP = 3;  // 拓展深度
constexpr int THREAD = 12;  // 线程最大限制

constexpr double C = 1;    //UCB 常数

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