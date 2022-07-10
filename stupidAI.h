#include "board.h"
#include "define.h"
#include "fun.h"

#pragma once

// TODO： 吃长链，留双交
/**
 * @brief 傻AI
 *
 * @param brd 棋盘，一定要用&
 * @param owner 下棋方
 */
inline void stupidAI(Board &brd, int owner)
{
    string res;
    bool flag = 1;
    LOC t;
    while (1)
    {
        //如果遇到可以C型格，eatCBoxs()会occline这条边，所以下面不需要条用occline()
        t = brd.eatCBoxs(owner);
        if (t.first == -1) break;
        res += change(t);
    }
    t = brd.getRandLine();
    if (t != LOC(-1, -1))
    {
        res += change(t);
        brd.occLine(owner, t);
    }
    cout << "move " << res.size() / 3 << " " << res << endl;
}

/**
 * @brief 随机下棋AI
 *
 * @param brd 棋盘，一定要用&
 * @param owner 下棋方
 */
inline void randAI(Board &brd, int owner)
{
    string res;
    bool flag = 1;
    while (flag)
    {
        // getRandLine()执行失败时返回{-1，-1};
        LOC t = brd.getRandLine();
        // t.first==-1,是t=={-1，-1}的简写
        if (t.first == -1) break;
        res += change(t);
        if (!brd.occLine(owner, t)) { flag = 0; }
    }
    cout << "move " << res.size() / 3 << " " << res << endl;
}
