/*
 *此头文件是存放一些常用的宏定义
 */
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <time.h>
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

// 为了和格子对应
#define EMPTY 0
#define BLACK 1
#define WHITE -1

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