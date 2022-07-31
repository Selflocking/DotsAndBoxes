#include "UCT.h"
#include "Node.h"
#include "assess.h"
#include "board.h"
#include "define.h"
#include <mutex>
#include <random>
#include <thread>

std::mutex mtx;

int getBoardWinner(Board &CB, int LatterPlayer)
{
    LOC a[60];
    BoxBoard Advanced(CB);
    if (Advanced.getFilterMoves(a) != 0)
        return 0;
    int w = Advanced.getBoardWinner(LatterPlayer);
    return w;
}

int getBoardWinner(Board &CB, int LatterPlayer,int &score)
{
    LOC a[60];
    BoxBoard Advanced(CB);
    if (Advanced.getFilterMoves(a) != 0)//未到终局
    {
       score=Advanced.getPlayerBoxes(LatterPlayer);//score为该节点的父结点拥有者即LatterPlayer得到格子数
       return 0;
    }
    //到终局
    int w = Advanced.getBoardWinner(LatterPlayer,score);//score为该节点的父结点拥有者即LatterPlayer得到格子数
    return w;
}

void preProcess2(Node *node)
{
    /*
     * 假设终局有一个长链，在长链的中间插入边会形成两个长C
     */
    /*
     * 长链即使不是最后一个也可以吃完
     */
    /* 预处理1
     * 双交不得不吃
     * 两格短C可以选择不吃
     * 三个短C可以吃完也可以留双交
     * 四格及以上长C除非下完棋局结束否则留双交
     * 四个死环要么占满，要么留两个双交
     * 六格死环应该是可以吃完或者留两个双交？
     * 八格及以上死环除非下完棋局结束否则留两个双交
     *
     * 当然，也有可能对方比较傻，提前开长链，暂不考虑
     */
    /*预处理2:
     * 如果我们是先手并且场上目前没有两格短链，那么我们应该先开环，再开长链，
     * 因为开环，对手如果想保持控制权，必须给我们留两个双交，而开长链，对手只能留双交
     */

    /* 拓展节点：
     * 两格短链和双交是交换先后手的关键
     */
    /*拓展节点：
     * 对于链与环相交，必须下链中的边，这样可以将链和环分割开来，因为如果我们在环中下边，这个特殊结构会转变成死树，利好对手
     *
     * 对于环与环相交，每个环里的边为等价边，如果两个环相同，则整个结构中所有边为等价边
     *
     * 对于链与链相交，每个链里的边为等价边，如果组成的链长度相同，则整个结构中所有边为等价边
     */

    /*特殊结构判断
     * 简单的特殊结构为长链和环，
     * 长链的每一个格子自由度都为2，但是头尾不是同一个格子
     * 环的每一个格子自由度都为2，但是头尾是同一个格子
     *
     * 复杂的特殊结构为链与环相交（包括一格的链与环相交），链与链相交，环与环相交
     * 交叉处的格子自由度等于周围链格和环格的数目和
     */
    /*特殊结构判断：
     * 对于更为复杂的情况，比如一条链既与链相交，又与环相交，
     * 还是能分割成更小的部分进行处理
     * 这就要求，存储特殊结构时，并不是将诸如链与链这样的特殊结构整个存储，而是存下单独的链和环，使用相交点来判断是否为复杂的特殊结构
     */
}

int getFilterMCWinner(Board &CB, int NextPlayer, int Filter_Range)
{
    int player = NextPlayer;
    while (CB.getFreeEdgeNum() != 0) //当还存在自由边的时候
    {
        player = rndFilterTurn(CB, player, false, Filter_Range); //#传入后续玩家#
    }
    int W = getBoardWinner(CB, -player);
    return W;
}

int getFilterMCWinner(Board &CB, int NextPlayer, int Filter_Range,int &score)
{
   int player = NextPlayer;
    while (CB.getFreeEdgeNum() != 0) //当还存在自由边的时候
    {
        player = rndFilterTurn(CB, player, false, Filter_Range); //#传入后续玩家#
    }
    int W = getBoardWinner(CB, -player,score);
    return W;
}

void multi_thread_func(Board &CB, int &a, int NextPlayer, int Winner, int Filter_Range)
{
    if (getFilterMCWinner(CB, NextPlayer, Filter_Range) == Winner) //#传入的是后续玩家#
    {
        mtx.lock();
        a++;
        mtx.unlock();
    }
}

float getFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range)
{
    Board MCB = CB; //先复制一个棋盘
    int MCE=0;//每次模拟所得收益值的总和
    int threadnum = 11;
    thread ths[11];
    //当前版本为11线程并行模拟
    for (int i = 0; i < TIMES; i++)
    {
        int CNT=0;//每次模拟该节点的父结点拥有者获得格子数
        int WIN=0;//每次模拟的结果，0或1
        if (getFilterMCWinner(MCB, NextPlayer, Filter_Range,CNT) == Winner) //#传入的是后续玩家#
          WIN++;
        MCE+=WIN+CNT-13;
    }
    // for (int i = 0; i < threadnum; i++)
    // {
    //     ths[i] = thread(multi_thread_func, ref(MCB), ref(MCE), NextPlayer, Winner, Filter_Range);
    // }
    // for (int i = 0; i < threadnum; i++)
    // {
    //     ths[i].join();
    // }
    // float score = ((float)MCE) / ((float)threadnum);
    float score = ((float)MCE) / ((float)TIMES);
    return score;
}

float UCTProcess(Node &B, int &Total, int MC_Times, int Filter_Range) //#Total 尝试次数#
{
    B.Times++;              //访问的次数增加一次
    if (B.BoardWinner != 0) //如果游戏已经结束了#叶节点#
    {
        if (B.BoardWinner == B.Owner)
            B.AvgValue = 0; //如果在这个节点游戏结束了，判定收益。
        else
            B.AvgValue = 1; //如果在这个节点游戏结束了，判定收益
        Total++;            //如果搜索到游戏结束的节点，则本次迭代结束。
        return B.AvgValue;
    }
    if (B.ExistChild < B.TotalChild) //如果还有未尝试过的节点
    {
        Total++; //基准情形，本次迭代结束，尝试次数+1。
        B.ChildNodes[B.ExistChild] = B.expandUCTNode(MC_Times, Filter_Range); //扩展一个子节点
        B.ExistChild++;                                                       //子节点的数目自增1
        B.refreshAvgValue();                                                  //刷新收益
        return B.AvgValue;
    }
    else //说明没有未尝试过的节点
    {
        int BestNodeNum = 0;
        double BestUCBValue = 0;
        double UCBValue[60];
        for (int i = 0; i < B.TotalChild; i++)
        {
            UCBValue[i] = B.ChildNodes[i]->getUCBValue(Total);
            if (UCBValue[i] >= BestUCBValue)
            {
                BestNodeNum = i;
                BestUCBValue = UCBValue[i];
            }
        }
        UCTProcess(*B.ChildNodes[BestNodeNum], Total, MC_Times, Filter_Range);
        B.refreshAvgValue();
        return B.AvgValue;
    }
    return 0;
}

void UCTMove(Board &CB, int Player, bool Msg, vector<LOC> &pace)
{
    Node UCTB = Node(Player, CB.map, true, UCT_FILTER_RANGE); //根据当前局面创建UCT的根节点
    if (UCTB.BoardWinner == 0)
    {
        int Total = 0; // UCT的次数函数
        clock_t start; //设置计时器的变量
        start = clock();
        for (int i = 0; i < UCT_TIMES; i++) //迭代一定次数
        {
            UCTProcess(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);

            double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
            if (totaltime >= UCT_LIMIT_TIME)
                break;
        }
        //判定最佳收益
        int BestNodeNum = 0;
        float BestAvgValue = 0;
        int LargerTimesNodeNum = 0;
        int LargerTimesValue = 0;
        for (int i = 0; i < UCTB.ExistChild; i++)
        {
            if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
            {
                BestNodeNum = i;
                BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
            }
            if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
            {
                LargerTimesNodeNum = i;
                LargerTimesValue = UCTB.ChildNodes[i]->Times;
            }
        }

        CB.move(Player, {UCTB.NodeMoves[BestNodeNum].first, UCTB.NodeMoves[BestNodeNum].second});
        pace.push_back({UCTB.NodeMoves[BestNodeNum].first, UCTB.NodeMoves[BestNodeNum].second}); //**记录步伐

        if (Msg)
        {
            cout << UCTB.NodeMoves[BestNodeNum].first << "," << UCTB.NodeMoves[BestNodeNum].second << " " << Player
                 << endl;
            cout << "========================Infomation==========================\n";
            cout << "当前节点平均收益为" << 1 - UCTB.AvgValue << endl;
            if (BestNodeNum == LargerTimesNodeNum)
                cout << "最大访问与最佳收益相同！\n";
            else
                cout << "最大访问不等同于最佳收益！\n";
            cout << "最佳收益节点访问为" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
            cout << "最佳收益节点收益为" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
            cout << "最多访问节点访问为" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
            cout << "最多访问节点收益为" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
            cout << "本次UCT总迭代次数为" << Total << endl;
            cout << "============================================================\n";
        }
        //释放内存
        deleteUCTTree(UCTB);
    }
    else
    {
        CB.eatAllCTypeBoxes(Player, pace);     //**记录步伐
        latterSituationMove(CB, Player, pace); //**记录步伐
    }
}

void deleteUCTNode(Node *Root)
{
    if (Root->ExistChild > 0)
    {
        for (int i = 0; i < Root->ExistChild; i++)
        {
            deleteUCTNode(Root->ChildNodes[i]);
            delete Root->ChildNodes[i];
        }
    }
}
void deleteUCTTree(Node Root)
{
    for (int i = 0; i < Root.ExistChild; i++)
    {
        deleteUCTNode(Root.ChildNodes[i]);
        delete Root.ChildNodes[i];
    }
}

void UCTMoveWithSacrifice(Board &CB, int Player, bool Msg, vector<LOC> &pace)
{

    BoxBoard Dead(CB);
    vector<LOC> tmp = pace;
    bool DeadChain = Dead.getDeadChainExist();
    bool DeadCircle = Dead.getDeadCircleExist();
    if (DeadCircle || DeadChain) //有环的情况优先处理
    {
        int SacrificeBoxNum;
        if (DeadChain)
            SacrificeBoxNum = 2;
        if (DeadCircle)
            SacrificeBoxNum = 4;

        //假设全部都吃掉了
        Dead.eatAllCTypeBoxes(Player, tmp);         //此处传的第二个参数无意义
        LOC BoxNum = Dead.getEarlyRationalBoxNum(); // x是先手的，y是后手的
        //现在根据接下来局面的估值函数来进行分析
        if (BoxNum.first - BoxNum.second <= SacrificeBoxNum) //放弃控制
        {
            CB.eatAllCTypeBoxes(Player, pace); //**记录步伐
            UCTMove(CB, Player, true, pace);   //**记录步伐
        }
        else
        {
            //牺牲，此时必有死长链或死环
            LOC DCMove;
            if (SacrificeBoxNum == 2) //然后处理死链
            {
                //首先吃到贪婪的临界点
                for (;;)
                {
                    Board Test = CB;
                    Test.eatCBox(Player); //模拟走一步
                    BoxBoard Dead(Test);
                    if (Dead.getDeadChainExist())
                    {
                        LOC t = CB.eatCBox(Player);
                        pace.push_back(t); //*记录步伐
                    }
                    else
                        break;
                }
                //然后开始考虑DoubleCross
                DCMove = CB.getDoubleCrossLoc(Player);
            }
            else
            {
                //首先吃到贪婪的临界点
                for (;;)
                {
                    Board Test = CB;
                    Test.eatCBox(Player);
                    BoxBoard Dead(Test);
                    if (Dead.getDeadCircleExist())
                    {
                        LOC t = CB.eatCBox(Player);
                        pace.push_back(t); //**记录步伐
                    }
                    else
                        break;
                }
                //然后开始考虑DoubleCross
                DCMove = CB.getDoubleCrossLoc(Player);
            }

            CB.move(Player, {DCMove.first, DCMove.second});
            pace.push_back(DCMove); //**记录步伐
            for (;;)                //吃掉所有死格
            {
                if (!CB.getCTypeBoxLimit(Player, pace)) //**记录步伐
                    break;
            }
            //牺牲结束
        }
    }
    else //正常UCT移动
    {
        CB.eatAllCTypeBoxes(Player, pace); //**记录步伐
        UCTMove(CB, Player, true, pace);   //**记录步伐
    }
}

void latterSituationMove(Board &CB, int Player, vector<LOC> &pace)
{
    //后期算法，此时只有长链和环。
    if (CB.getLongCTypeBoxExist())
    {
        //已有打开的长链，根据牺牲与不牺牲之后的理性状态决定是否牺牲。
        BoxBoard Dead(CB);
        int SacrificeBoxNum = 0;
        if (Dead.getDeadChainExist())
            SacrificeBoxNum = 2;
        if (Dead.getDeadCircleExist())
            SacrificeBoxNum = 4;

        //假设全部都吃掉了
        Dead.eatAllCTypeBoxes(Player);              //此处传的第二个参数无意义
        LOC BoxNum = Dead.getRationalStateBoxNum(); // x是先手的，y是后手的
        cout << "BoxNum:" << BoxNum.first << " " << BoxNum.second << endl;
        cout << "Sacri:" << SacrificeBoxNum << endl;
        //现在根据接下来局面能得到的格子数来进行分析
        //假设在当前链全被消灭后后手可以拿到x个，先手可以拿到y个。我方全吃后可以拿到y+n，对方拿到x个。若x-y<n则我方全吃
        if (BoxNum.first - BoxNum.second <= SacrificeBoxNum || Dead.getWinner() != EMPTY)
        {
            //放弃控制
            cout << "Winner:" << Dead.getWinner() << endl;
            CB.eatAllCTypeBoxes(Player, pace); //**记录步伐
            if (Dead.getWinner() == EMPTY)
                latterSituationMove(CB, Player, pace); //**记录步伐
        }
        else
        {
            //牺牲，此时必有死长链或死环
            LOC DCMove;
            if (SacrificeBoxNum == 2) //然后处理死链
            {
                //首先吃到贪婪的临界点
                for (;;)
                {
                    Board Test = CB;
                    Test.eatCBox(Player); //模拟走一步
                    BoxBoard Dead(Test);
                    if (Dead.getDeadChainExist())
                    {
                        LOC tmp = CB.eatCBox(Player);
                        pace.push_back(tmp); //**记录步伐
                    }
                    else
                        break;
                }
                //然后开始考虑DoubleCross
                DCMove = CB.getDoubleCrossLoc(Player);
            }
            else
            {
                //首先吃到贪婪的临界点
                for (;;)
                {
                    Board Test = CB;
                    Test.eatCBox(Player);
                    BoxBoard Dead(Test);
                    if (Dead.getDeadCircleExist())
                    {
                        LOC t = CB.eatCBox(Player);
                        pace.push_back(t); //**记录步伐
                    }
                    else
                        break;
                }
                //然后开始考虑DoubleCross
                DCMove = CB.getDoubleCrossLoc(Player);
            }

            CB.move(Player, DCMove);
            pace.push_back(DCMove); //**记录步伐
            for (;;)                //直到无法占据CTypeBox了就结束
            {
                if (!CB.getCTypeBoxLimit(Player, pace)) //**记录步伐
                    break;
            }
            //牺牲结束
        }
    }
    else
    {
        //选择打开哪一条长链。根据理性状态决定是打开最短的长链还是如何
        CB.eatAllCTypeBoxes(Player, pace); //**记录步伐
        BoxBoard Test(CB);

        LOC p;
        p = Test.openPolicy(); //调用打开决策函数获得打开步伐坐标
        CB.move(Player, p);
        pace.push_back(p); //**记录步伐
    }
}

//游戏移动，会根据前中后期自动移动
void gameTurnMove(Board &CB, int Player, bool Msg, int *status, vector<LOC> &pace)
{
    // This Function is using for the game's move turn.

    Board Test = CB;
    Test.eatAllCTypeBoxes(Player);
    bool LatterSituation = (Test.getFilterMoveNum() == 0);
    if (!LatterSituation)
        UCTMoveWithSacrifice(CB, Player, Msg, pace);
    else //也就是后期局面了
    {
        //也就是Filter都已经无能为力的情况下，只有LongChain,Circle,PreCircle
        latterSituationMove(CB, Player, pace); //**记录步伐
    }
    *status = 1;
}

int rndFilterTurn(Board &CB, int Player, bool Msg, int Filter_Range)
{
    LOC Moves[60];
    vector<LOC> t;
    CB.eatAllCTypeBoxes(Player, t); //此处第二个参数无实际意义

    BoxBoard Test = CB;
    int MoveNum;
    int FreeEdge = CB.getFreeEdgeNum();
    if (FreeEdge < Filter_Range)              //仅在FreeEdge数量小于25的情况下考虑Filter（过滤）
        MoveNum = Test.getFilterMoves(Moves); //确定这个局面下MoveNum的数量
    else
        MoveNum =Test.getFreeMoves(Moves); //确定这个局面下MoveNum的数量

    if (MoveNum !=
        0) //在某些时候，由于吃掉了前面的C型格。可能导致MoveNum的数量为0.这时候只要跳过这一步自然就会开始判断胜利。
    {
        int Rnd = rand() % MoveNum; //在0-MoveNum中抽取一个随机数
        CB.move(Player, {Moves[Rnd].first, Moves[Rnd].second});
        return -Player; //换手
    }
    else
    {
        return Player; //不换手
    }
}