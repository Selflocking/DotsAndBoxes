#include "datarecorder.h"

data_of_game::data_of_game(int a, int b,const char* p1,const char* p2)
{
    root = cJSON_CreateObject(); // json根节点
    piece = cJSON_CreateArray(); // json落子信息数组节点
    // objarr = cJSON_CreateObject(); // json的对象节点用于将树添加入json——root
    cJSON_AddStringToObject(root, "R", p1);
    cJSON_AddStringToObject(root, "B", p2);
    char *win;
    if (a>b)strcpy(win,"R");
    else strcpy(win,"B");
    cJSON_AddStringToObject(root, "winner", win);
    cJSON_AddNumberToObject(root, "RScore", a);
    cJSON_AddNumberToObject(root, "BScore", b);
    time_t t1 = time(nullptr);
    char *res;
    res = ctime(&t1);
    cJSON_AddStringToObject(root, "Date", "2022-08-12");
    cJSON_AddStringToObject(root, "Event", "2022 CCGC");
}

void data_of_game::recordstep(Step v[60])//x行y列
{
    for (int i = 0;i < 60;i ++)
    {
        if (v[i].player == EMPTY)continue;
        string tem;
        tem += v[i].player == -1?"b(":"r(";
        tem += (v[i].action.second/2 +'a');
        tem += ( 6-((v[i].action.first+1)/2)+'0');
        tem += (v[i].action.first % 2 == 0 ? ",h)":",v)");
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
void data_of_game::printdata(string fir, string sec, string winner)
{
    // char *json_data = cJSON_Print(root);
    char *json_data = cJSON_PrintUnformatted(root);
    fstream out;
    string filename = fir + " VS ";
    filename = filename + sec + " " + winner + ".txt";
    out.open(filename, ios::out);
    cout << filename;
    out << json_data;
    out.close();
}

void data_of_game::deleterecord() //释放json节点的空间
{
    cJSON_Delete(root);
    // cJSON_Delete(objarr);
}

