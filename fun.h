#include "define.h"
#pragma once
//一条线 LOC 转 string
inline string change(LOC l) {
    string s;
    if (l.first % 2 == 0 && l.second % 2 == 1) {
        s += 'A';
        s += 'A' + l.first / 2;
        s += 'A' + l.second / 2;
    } else {
        s += 'B';
        s += 'A' + l.second / 2;
        s += 'A' + l.first / 2;
    }
    return s;
}

//一条线 string 转 LOC
inline LOC change(string l) {
    LOC res;
    if (l[0] == 'A') {
        res.first = (l[1] - 'A') * 2;
        res.second = (l[2] - 'A') * 2 + 1;
    } else {
        res.second = (l[1] - 'A') * 2;
        res.first = (l[2] - 'A') * 2 + 1;
    }
    return res;
}