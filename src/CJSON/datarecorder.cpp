#include "datarecorder.h"
#include <fstream>
#include <nowide/convert.hpp>

using std::fstream;
using std::ios;

data_of_game::data_of_game(int a, int b, const char *p1, const char *p2)
{
    root = cJSON_CreateObject(); // json根节点
    piece = cJSON_CreateArray(); // json落子信息数组节点
    cJSON_AddStringToObject(root, "R", p1);
    this->pf = p1;
    cJSON_AddStringToObject(root, "B", p2);
    this->ps = p2;
    if (a + b == 25)
    {
        if (a > b)
        {
            winner = "先手胜";
            win = "R";
        }
        else
        {
            winner = "后手胜";
            win = "B";
        }
    }
    else
    {
        winner = "比赛未完成";
        win = "U";
    }
    cJSON_AddStringToObject(root, "winner", win);
    cJSON_AddNumberToObject(root, "RScore", a);
    cJSON_AddNumberToObject(root, "BScore", b);
    time_t now = time(nullptr);
    tm *date = localtime(&now);
    char buf[24];
    sprintf(buf, "%d-%d-%d", date->tm_year, date->tm_mon, date->tm_mday);
    cJSON_AddStringToObject(root, "Date", buf);
    cJSON_AddStringToObject(root, "Event", "2022 CCGC");
}

void data_of_game::recordstep(Step v[60]) // x行y列
{
    for (int i = 0; i < 60; i++)
    {
        if (v[i].player == EMPTY)
            continue;
        std::string tem;
        tem += v[i].player == -1 ? "b(" : "r(";
        tem += (v[i].action.second / 2 + 'a');
        tem += (6 - ((v[i].action.first + 1) / 2) + '0');
        tem += (v[i].action.first % 2 == 0 ? ",h)" : ",v)");
        cJSON *node = cJSON_CreateObject();
        cJSON_AddStringToObject(node, "piece", tem.c_str());
        cJSON_AddItemToArray(piece, node);
    }
}

void data_of_game::endrecord()
{
    cJSON_AddItemToObject(root, "game", piece);
}

//打印生成的结果
void data_of_game::printdata()
{
    char *json_data = cJSON_PrintUnformatted(root);
    fstream out;
    char buf[64];
    sprintf(buf, "DB：%s vs %s ：%s.txt", pf, ps, winner);
    out.open(nowide::widen(buf).c_str(), ios::out);
    out << json_data;
    // cout << filename;
    out.close();
}

void data_of_game::deleterecord() //释放json节点的空间
{
    cJSON_Delete(root);
}
