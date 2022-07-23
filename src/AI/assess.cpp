#include "assess.h"

BoxType::BoxType()
{
    Type = 0;              //默认类型：未被占领
    BelongingChainNum = 0; //默认为必空的0号空间
}

ChainInfo::ChainInfo()
{
    Type = NotDefine;             //类型为未定义
    ChainBoxNum = 0;              //所属格子数在开始时候为0
    ConditionOfPreCircle = false; //是否预备环的先决条件
}

BoxBoard::BoxBoard(Board NewB)
{
    //复制基本属性
    for (int i = 0; i < LEN; i++)
    {
        for (int j = 0; j < LEN; j++)
        {
            map[i][j] = NewB.map[i][j];
        }
    }
}

BoxBoard::BoxBoard(int m[LEN][LEN], int step)
{
    for (int i = 0; i < LEN; i++)
    {
        for (int j = 0; j < LEN; j++)
        {
            map[i][j] = m[i][j];
        }
    }
}

void BoxBoard::resetChainsInfo() //重置链与格的信息，不使用0号空间
{
    for (int y = 1; y <= BOXLEN; y++)
    {
        for (int x = 1; x <= BOXLEN; x++)
        {
            Boxes[x][y].BelongingChainNum = 0; //初始化格子所属链类型的编号
        }
    }
    for (int i = 1; i < 25; i++)
    {
        Chains[i].ChainBoxNum = 0;
        Chains[i].Type = NotDefine;
        Chains[i].ConditionOfPreCircle = false; //预备环的先决条件
    }
}

void BoxBoard::searchingFromBox(LOC BoxLoc) //从一个格出发，注册他的所有派生链，ChainPlus应在没有短链时启用
{
    // cout<<"进入searchingFromBox函数"<<endl;
    int Dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for (int n = 0; n < 4; n++)
    {
        int bx = BoxLoc.first + Dir[n][0];
        int by = BoxLoc.second + Dir[n][1]; //#邻格#
        int lx = (BoxLoc.first * 2) - 1 + Dir[n][0];
        int ly = (BoxLoc.second * 2) - 1 + Dir[n][1];                                      //#邻边#
        if ((map[lx][ly] == HENG || map[lx][ly] == SHU) && getBoxType(bx, by) == CHAINBOX) //邻边为空，目标格子为链格
        {
            if (Boxes[bx][by].BelongingChainNum == EMPTY) //必须为未归属的格子，避免环重复从不同方向出发。
            {
                LOC NewLoc;
                NewLoc = {bx, by};
                registerChain(BoxLoc, NewLoc);
            }
        }
    }
    // cout<<"完成searchingFromBox函数"<<endl;
}

int BoxBoard::getFirstEmptyChainNum() //获取可用链空间的编号
{
    for (int i = 1; i < 25; i++) //不采用0号空间
    {
        if (Chains[i].Type == NotDefine)
            return i;
    }
    cout << "链空间不足！" << endl;
    system("pause");
    return 0;
}

void BoxBoard::registerChain(LOC FreeBoxLoc, LOC FirstLoc) //动态注册链。一般仅从自由格出发
{
    cout << "进入registerChain" << endl;
    int x = FreeBoxLoc.first;         //初始X值
    int y = FreeBoxLoc.second;        //初始Y值
    LOC Loc = FreeBoxLoc;             //设置起点坐标
    LOC NewLoc = FirstLoc;            //设置新坐标
    int Re = getFirstEmptyChainNum(); //获取一个空白的链空间
    Chains[Re].StartLoc = Loc;        //起点为某个自由格
    Chains[Re].ChainBoxNum = 0;       //更新起点
    Chains[Re].Type = NotDefine;

    LOC side = getPublicSide(FirstLoc, FreeBoxLoc);
    Chains[Re].edgeLoc.push_back(side); //把该链中的第一条边压进数组

    //在如下过程中，NewLoc是新找到的格子的坐标。链的类型在搜索结束时候判断
    for (int i = 0; i < 25; i++) //
    {
        if (getBoxType(NewLoc.first, NewLoc.second) !=
            CHAINBOX) //当搜索结束的时候，判定链的类型。此时，NewLoc为搜索到的终点格坐标
        {
            Chains[Re].EndLoc = NewLoc; //搜索到达终点
            if (Boxes[NewLoc.first][NewLoc.second].Type == DEADBOX)
                inheritChain(EMPTY, Re);
            else if (NewLoc.first == FreeBoxLoc.first && NewLoc.second == FreeBoxLoc.second &&
                     Boxes[NewLoc.first][NewLoc.second].BelongingChainNum == EMPTY)
            {
                //若终点格的坐标与起点格相同并且该格还未被认定类型，则认定为PreCircle
                Chains[Re].Type = PreCircle;
                Boxes[NewLoc.first][NewLoc.second].BelongingChainNum = Re; //包括该格在内也需要被注册
                Chains[Re].ChainBoxNum++;
                cout << "长度为：" << Chains[Re].ChainBoxNum << endl;
            }
            else //若起终点不一样，则为链。根据数目判定其类型
            {
                if (Chains[Re].ChainBoxNum == 1 || Chains[Re].ChainBoxNum == 2)
                    Chains[Re].Type = ShortChain;
                else
                    Chains[Re].Type = LongChain;
                cout << "长度为：" << Chains[Re].ChainBoxNum << "类型为：" << Chains[Re].Type << endl;
            }
            break;
        }
        else //每搜到一个新的格子
        {
            LOC mLoc = NewLoc;
            //定义格子的信息
            Boxes[NewLoc.first][NewLoc.second].BelongingChainNum = Re; //与链挂钩
            Chains[Re].ChainBoxNum++;                                  //链的格子数目自增1
            NewLoc = findNextBox(NewLoc, Loc);                         //寻找下一个。
            Loc = mLoc;

            side = getPublicSide(NewLoc, Loc);
            Chains[Re].edgeLoc.push_back(side); //每搜索到一个新格子存进一条边

            cout << "长度为：" << Chains[Re].ChainBoxNum << "类型为：" << Chains[Re].Type << endl;
        }
    }
    cout << "完成registerChain" << endl;
}

LOC BoxBoard::findNextBox(LOC FoundBox, LOC LastBox)
{
    //查找某个链格,给出的坐标可能包含超区坐标。请使用前利用GetBoxType鉴定
    int fbx = FoundBox.first * 2 - 1;
    int fby = FoundBox.second * 2 - 1; //#转换成board坐标#
    LOC AimBox;
    if (map[fbx + 1][fby] != OCCLINE && ((FoundBox.first + 1) != LastBox.first || (FoundBox.second) != LastBox.second))
    {
        // 0度  FoundBox右邻边为空，同时右目标格子不是上上个找到的格子LastBox
        AimBox = {FoundBox.first + 1, FoundBox.second}; //#确定目标格子#
        return AimBox;
    }
    else if (map[fbx][fby + 1] != OCCLINE &&
             ((FoundBox.first) != LastBox.first || (FoundBox.second + 1) != LastBox.second))
    {
        // 90度  FoundBox下邻边为空，同时下目标格子不是上上个找到的格子LastBox
        AimBox = {FoundBox.first, FoundBox.second + 1};
        return AimBox;
    }
    else if (map[fbx - 1][fby] != OCCLINE &&
             ((FoundBox.first - 1) != LastBox.first || (FoundBox.second) != LastBox.second))
    {
        // 180度  FoundBox左邻边为空，同时左目标格子不是上上个找到的格子LastBox
        AimBox = {FoundBox.first - 1, FoundBox.second};
        return AimBox;
    }
    else if (map[fbx][fby - 1] != OCCLINE &&
             ((FoundBox.first) != LastBox.first || (FoundBox.second - 1) != LastBox.second))
    {
        // 270度  FoundBox上邻边为空，同时上目标格子不是上上个找到的格子
        AimBox = {FoundBox.first, FoundBox.second - 1};
        return AimBox;
    }
    AimBox = {0, 0}; //代表错误
    return AimBox;
}

int BoxBoard::getBoxType(int bx, int by) //从棋盘上获取格子类型
{
    cout << "进入getBoxType" << endl;
    if (bx >= 1 && by >= 1 && bx <= BOXLEN && by <= BOXLEN) //满足格子要求
        return Boxes[bx][by].Type;
    return FREEBOX; //其余超出范围的均返回自由格
    cout << "完成getBoxType" << endl;
}

void BoxBoard::defineAllChains(bool ChainPlus) //定义所有的链
{
    cout << "进入defineAllChains" << endl;
    // defineBoxesType(); //首先定义所有格子的类型
    resetChainsInfo(); //重置链的数据

    //首先从自由格出发搜索所有长链
    for (int i = 1; i <= BOXLEN; i++)
    {
        for (int j = 1; j <= BOXLEN; j++)
        {
            if (Boxes[i][j].Type == 3)
            {
                LOC k;
                k = {i, j};
                searchingFromBox(k);
            }
        }
    }

    //然后在边缘搜索还没有被排查的链
    for (int i = 1; i <= BOXLEN; i++)
    {
        LOC k;
        k = {i, 0};
        searchingFromBox(k);
    }
    for (int i = 1; i <= BOXLEN; i++)
    {
        LOC k;
        k = {0, i};
        searchingFromBox(k);
    }
    for (int i = 1; i <= BOXLEN; i++)
    {
        LOC k;
        k = {i, BOXLEN + 1};
        searchingFromBox(k);
    }
    for (int i = 1; i <= BOXLEN; i++)
    {
        LOC k;
        k = {BOXLEN + 1, i};
        searchingFromBox(k);
    }

    //然后搜索还没有被搜索到的链格，可能为环
    for (int i = 1; i <= BOXLEN; i++)
    {
        for (int j = 1; j <= BOXLEN; j++)
        {
            if (Boxes[i][j].Type == CHAINBOX && Boxes[i][j].BelongingChainNum == EMPTY)
            {
                LOC BoxLoc;
                BoxLoc = {i, j};
                searchingCircle(BoxLoc);
            }
        }
    }

    //暂时没用到，定义与预备链和合并自由格相连的两条链
    //最后开始ChainPlus
    if (ChainPlus)
    {
        //首先查找是否有首尾相同的链，定义为PreChain
        for (int i = 1; i < 25; i++)
        {
            if (Chains[i].Type == LongChain)
            {
                for (int j = i + 1; j <= 25; j++)
                {
                    if (Chains[j].Type == LongChain && Chains[i].StartLoc == Chains[j].StartLoc &&
                        Chains[i].EndLoc == Chains[j].EndLoc)
                    {
                        if (Boxes[Chains[j].StartLoc.first][Chains[j].StartLoc.second].BelongingChainNum == EMPTY &&
                            Boxes[Chains[j].EndLoc.first][Chains[j].EndLoc.second].BelongingChainNum == EMPTY)
                        {
                            //此时，i与j同为长链而且首尾相同且首尾的格子都未被认定，可以认定他们为PreChain
                            inheritChain(i, j); //链i继承了链j
                            Boxes[Chains[i].StartLoc.first][Chains[i].StartLoc.second].BelongingChainNum = i;
                            Boxes[Chains[i].EndLoc.first][Chains[i].EndLoc.second].BelongingChainNum = i;
                            Chains[i].ChainBoxNum = Chains[i].ChainBoxNum + 2;
                            Chains[i].StartLoc = Chains[i].EndLoc;
                            Chains[i].Type = PreCircle;
                        }
                    }
                }
            }
        }

        //然后把所有PreCircle的PreChain定义好
        for (int y = 1; y <= BOXLEN; y++)
        {
            for (int x = 1; x <= BOXLEN; x++)
            {
                if (Boxes[x][y].Type == 3 && Chains[Boxes[x][y].BelongingChainNum].Type == PreCircle)
                {
                    int Dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
                    int bx = x * 2 - 1;
                    int by = y * 2 - 1;
                    //先手先产生一个死格
                    for (int n = 0; n < 4; n++)
                    {
                        int ex = bx + Dir[n][0];
                        int ey = by + Dir[n][1]; //#邻边#
                        int nbx = x + Dir[n][0];
                        int nby = y + Dir[n][1]; //#邻格#
                        if (map[ex][ey] != OCCLINE && Chains[Boxes[nbx][nby].BelongingChainNum].Type == LongChain)
                        {
                            Chains[Boxes[nbx][nby].BelongingChainNum].ConditionOfPreCircle = true;
                        }
                    }
                }
            }
        }

        //最后把所有还没定义的自由格归属到他最长的两条派生链的合并中
        for (int y = 1; y <= BOXLEN; y++)
        {
            for (int x = 1; x <= BOXLEN; x++)
            {
                int Dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
                if (Boxes[x][y].Type == FREEBOX && Boxes[x][y].BelongingChainNum == EMPTY) //若本自由格还未确定
                {
                    int ChainRegNum[4];
                    int Total = 0;
                    for (int n = 0; n < 4; n++)
                    {
                        int bx = x + Dir[n][0];
                        int by = y + Dir[n][1];
                        int lx = (x * 2) - 1 + Dir[n][0];
                        int ly = (y * 2) - 1 + Dir[n][1];
                        if (map[lx][ly] != OCCLINE && getBoxType(bx, by) == CHAINBOX) //邻边为空，目标格子为链格
                        {
                            ChainRegNum[Total] = Boxes[bx][by].BelongingChainNum;
                            Total++;
                        }
                    }
                    //先找出最长的
                    int FirstChainNum = 0;
                    int FirstChainBoxes = 0;
                    for (int i = 0; i < Total; i++)
                    {
                        if (Chains[ChainRegNum[i]].ChainBoxNum > FirstChainBoxes)
                        {
                            FirstChainNum = ChainRegNum[i];
                            FirstChainBoxes = Chains[ChainRegNum[i]].ChainBoxNum;
                        }
                    }
                    //再找出次长的
                    int SecChainNum = 0;
                    int SecChainBoxes = 0;
                    for (int i = 0; i < Total; i++)
                    {
                        if (Chains[ChainRegNum[i]].ChainBoxNum > SecChainBoxes && ChainRegNum[i] != FirstChainNum)
                        {
                            SecChainNum = ChainRegNum[i];
                            SecChainBoxes = Chains[ChainRegNum[i]].ChainBoxNum;
                        }
                    }
                    //将两个链，以及本格累加在一起
                    Boxes[x][y].BelongingChainNum = FirstChainNum;
                    Chains[FirstChainNum].ChainBoxNum++;
                    inheritChain(FirstChainNum, SecChainNum);
                }
            }
        }
    }
}

void BoxBoard::searchingCircle(LOC BoxLoc)
{
    int Dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for (int n = 0; n < 4; n++)
    {
        int bx = BoxLoc.first + Dir[n][0];
        int by = BoxLoc.second + Dir[n][1];
        int lx = (BoxLoc.first * 2) - 1 + Dir[n][0];
        int ly = (BoxLoc.second * 2) - 1 + Dir[n][1];
        if (map[lx][ly] != OCCLINE && getBoxType(bx, by) == CHAINBOX) //邻边为空，目标格子为链格
        {
            if (Boxes[bx][by].BelongingChainNum == EMPTY) //必须为未归属的格子，避免环重复从不同方向出发。
            {
                LOC NewLoc;
                NewLoc = {bx, by};
                registerCircle(BoxLoc, NewLoc);
            }
        }
    }
}

void BoxBoard::registerCircle(LOC StartLoc, LOC NextLoc) //动态注册链.一般仅从自由格出发
{
    cout << "进入registerCircle函数" << endl;
    int x = StartLoc.first;           //初始X值
    int y = StartLoc.second;          //初始Y值
    LOC Loc = StartLoc;               //设置起点坐标
    LOC NewLoc = NextLoc;             //设置新坐标
    int Re = getFirstEmptyChainNum(); //获取一个空白的链空间
    Chains[Re].ChainBoxNum = 0;       //改为0
    Chains[Re].Type = NotDefine;
    //在如下过程中，NewLoc才是实际链的每一个格子的坐标。链的类型在搜索结束时候判断
    for (int i = 0; i < 25; i++)
    {
        if (Boxes[NewLoc.first][NewLoc.second].Type != 2) //若非链格，则取消注册。
        {
            inheritChain(EMPTY, Re); // 0号链吞并所有已经注册的链
            break;
        }
        else if (NewLoc.first == StartLoc.first && NewLoc.second == StartLoc.second) //搜到起点，确定为Circle
        {
            Chains[Re].Type = Circle;
            Chains[Re].StartLoc = NewLoc;                              //起点与终点都标记为这个格子
            Chains[Re].EndLoc = NewLoc;                                //起点与终点都标记为这个格子
            Boxes[NewLoc.first][NewLoc.second].BelongingChainNum = Re; //包括该格在内也需要被注册
            Chains[Re].ChainBoxNum++;                                  //链的格子数目自增1
            break;
        }
        else //每搜到一个新的格子
        {
            LOC mLoc = NewLoc;
            //定义格子的信息
            Boxes[NewLoc.first][NewLoc.second].BelongingChainNum = Re; //与链挂钩
            Chains[Re].ChainBoxNum++;                                  //链的格子数目自增1
            NewLoc = findNextBox(NewLoc, Loc);                         //寻找下一个。
            Loc = mLoc;
        }
    }
    cout << "完成registerCircle函数" << endl;
}

void BoxBoard::inheritChain(int InheritorRegNum, int AncesterRegNum)
{
    cout << "进入inheritChain函数" << endl;
    for (int j = 1; j <= BOXLEN; j++)
    {
        for (int i = 1; i <= BOXLEN; i++)
        {
            if (Boxes[i][j].BelongingChainNum == AncesterRegNum)
                Boxes[i][j].BelongingChainNum = InheritorRegNum; //被继承
        }
    }
    Chains[InheritorRegNum].ChainBoxNum += Chains[AncesterRegNum].ChainBoxNum; //格子数累加
    Chains[InheritorRegNum].Type = LongChain;                                  //必定是长链。

    //销毁被继承者的注册空间
    Chains[AncesterRegNum].ChainBoxNum = 0;
    Chains[AncesterRegNum].Type = NotDefine;
    cout << "完成inheritChain函数" << endl;
}

void BoxBoard::defineDeadChain()
{
    cout << "进入defineDeadChain函数" << endl;
    // defineBoxesType(); //首先定义所有格子的类型
    resetChainsInfo(); //重置链的数据
    for (int i = 1; i <= BOXLEN; i++)
    {
        for (int j = 1; j <= BOXLEN; j++)
        {
            if (Boxes[i][j].Type == DEADBOX)
            {
                LOC k;
                k = {i, j};
                searchingDeadChain(k);
            }
        }
    }
    cout << "完成defineDeadChain函数" << endl;
}

void BoxBoard::searchingDeadChain(LOC BoxLoc)
{
    cout << "进入searchingDeadChain" << endl;
    int Dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for (int n = 0; n < 4; n++)
    {
        int bx = BoxLoc.first + Dir[n][0];
        int by = BoxLoc.second + Dir[n][1];
        int lx = (BoxLoc.first * 2) - 1 + Dir[n][0];
        int ly = (BoxLoc.second * 2) - 1 + Dir[n][1];
        if ((map[lx][ly] == HENG || map[lx][ly] == SHU) && getBoxType(bx, by) == CHAINBOX) //邻边为空，目标格子为链格
        {
            if (Boxes[bx][by].BelongingChainNum == EMPTY) //必须为未归属的格子，避免环重复从不同方向出发。
            {
                LOC NewLoc;
                NewLoc = {bx, by};
                registerDeadChain(BoxLoc, NewLoc);
            }
        }
    }
    cout << "完成searchingDeadChain" << endl;
}

void BoxBoard::registerDeadChain(LOC FreeBoxLoc, LOC FirstLoc)
{
    //动态注册链。从一个格子出发 ，向一个格子开始进行链的注册。
    cout << "进入registerDeadChain" << endl;

    int x = FreeBoxLoc.first;         //初始X值
    int y = FreeBoxLoc.second;        //初始Y值
    LOC Loc = FreeBoxLoc;             //设置起点坐标
    LOC NewLoc = FirstLoc;            //设置新坐标
    int Re = getFirstEmptyChainNum(); //获取一个空白的链空间
    Chains[Re].StartLoc = Loc;        //起点为某个自由格
    Chains[Re].ChainBoxNum = 1;       //更新起点
    Chains[Re].Type = NotDefine;

    //在如下过程中，NewLoc才是实际链的每一个格子的坐标。链的类型在搜索结束时候判断
    for (int i = 0; i < BOXNUM; i++)
    {
        if (getBoxType(NewLoc.first, NewLoc.second) !=
            CHAINBOX) //当搜索结束的时候，判定链的类型。此时，NewLoc为搜索到的终点格坐标
        {
            Chains[Re].EndLoc = NewLoc; //搜索到达终点
            if (getBoxType(NewLoc.first, NewLoc.second) ==
                DEADBOX) //如果终端是一个死格的话，这就是一个死循环(DeadCircle)
            {
                Chains[Re].Type = DeadCircle;
                Boxes[NewLoc.first][NewLoc.second].BelongingChainNum = Re; //包括该格在内也需要被注册
                Chains[Re].ChainBoxNum++;
            }
            else //若起终点不一样，则为链。根据数目判定其类型
            {
                Chains[Re].Type = DeadChain;
            }
            break;
        }
        else //每搜到一个新的格子
        {
            LOC mLoc = NewLoc;
            //定义格子的信息
            Boxes[NewLoc.first][NewLoc.second].BelongingChainNum = Re; //与链挂钩
            Chains[Re].ChainBoxNum++;                                  //链的格子数目自增1
            NewLoc = findNextBox(NewLoc, Loc);                         //寻找下一个。
            Loc = mLoc;
        }
    }

    cout << "完成registerDeadChain" << endl;
}