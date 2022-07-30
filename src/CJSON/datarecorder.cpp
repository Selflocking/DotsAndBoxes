#include "datarecorder.h"

data_of_game::data_of_game(int a, int b,string p1,string p2)
{
    root = cJSON_CreateObject(); // json根节点
    piece = cJSON_CreateArray(); // json落子信息数组节点
    // objarr = cJSON_CreateObject(); // json的对象节点用于将树添加入json——root
    const char *tem = p1.c_str();
    const char *tem2 = p2.c_str();
    const char *tem3;
    cJSON_AddStringToObject(root, "R", tem);
    this->pf = p1;
    // strcpy(tem,p2.c_str());
    cJSON_AddStringToObject(root, "B", tem2);
    this->ps = p2;
    if (a > b)
    {
        winner = "A";
        win = "R";
        tem3 = "R";
    }
    else if (winner.empty())
    {
        winner = "C";
        win = "F";
        tem3 = "U";
    }
    else
    {
        winner = "B";
        win = "B";
        tem3 = "B";
    }
    cJSON_AddStringToObject(root, "winner", tem3);
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
void data_of_game::printdata()
{
    // char *json_data = cJSON_Print(root);
    char *json_data = cJSON_PrintUnformatted(root);
    fstream out;
    string filename = "DB " + pf + " vs ";
    filename = filename + ps + " " + winner + ".txt";
    // filename = "tem.txt";
    out.open(filename, ios::out);
    out << json_data;
    cout << filename;
    out.close();
}

void data_of_game::deleterecord() //释放json节点的空间
{
    cJSON_Delete(root);
    // cJSON_Delete(objarr);
    // system("pause");
}

