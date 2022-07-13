#include "fun.h"

/**
 * @brief 将一条线的坐标转换成字符串
 * @param l 线的坐标值
 * @return 对阵平台通信协议需要的字符串
 */
string change(LOC l)
{
    string s;
    if (l.first % 2 == 0 && l.second % 2 == 1)
    {
        s += 'A';
        s += 'A' + l.first / 2;
        s += 'A' + l.second / 2;
    }
    else
    {
        s += 'B';
        s += 'A' + l.second / 2;
        s += 'A' + l.first / 2;
    }
    return s;
}

/**
 * @brief 将字符串转换成坐标
 * @param l 对战平台传进的坐标值
 * @return 坐标值
 */
LOC change(string l)
{
    LOC res;
    if (l[0] == 'A')
    {
        res.first = (l[1] - 'A') * 2;
        res.second = (l[2] - 'A') * 2 + 1;
    }
    else
    {
        res.second = (l[1] - 'A') * 2;
        res.first = (l[2] - 'A') * 2 + 1;
    }
    return res;
}