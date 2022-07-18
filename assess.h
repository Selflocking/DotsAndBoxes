#include "define.h"
#include "board.h"
#include <windows.h>

#pragma once

//各种链的编号  未定义，长链，锻炼，环，预备环，死链，死环
enum ChainType
{
	NotDefine, LongChain, ShortChain, Circle, PreCircle, DeadChain, DeadCircle
};

//BoxType类是一个用于鉴定格子类型的类
class BoxType
{
public:
	BoxType();
	LOC BoxLoc;
	int BoxOwner;			    //格子的属主，大部分格子没有属主
	int Type;				    //[0]已被占领 [1]死格 [2]链格 [3]自由格
	int BelongingChainNum;		//所属链类型的编号，会从1开始自动编号。
};

//ChainInfo是一个包含了一条链的基本信息的类
class ChainInfo
{
public:
	ChainInfo();
	ChainType Type;
	int ChainBoxNum;//#属于多少个格子#
	LOC StartLoc;
	LOC EndLoc;
	bool ConditionOfPreCircle;//#预备环的先决条件#
};

class BoxBoard :public Board
{
public:
	BoxBoard(Board NewB);				
	BoxBoard(int map[LEN][LEN], int step);
	BoxType Boxes[BOXLEN + 1][BOXLEN + 1];//是格子数组。后期棋盘的基本类型(*占领/未被占领*)，编号从1开始。
	ChainInfo Chains[25];  //链的数量必定不会超过全部格子数

    LOC findNextBox(LOC FoundBox, LOC LastBox);	//查找某个链格 
	int getFirstEmptyChainNum();//功能：获得一个空的链编号 
	int getBoxType(int bx, int by);
	//用于得到某个格子的类型  不在坐标范围内的全部返回自由格类型[坐标限制为1到BOXLEN]。

	//鉴别链
	void inheritChain(int InheritorRegNum, int AncesterRegNum);	//一条链吞并另一条链 
	void registerChain(LOC FreeBoxLoc, LOC NextLoc);			
	//从一个场内的自由格出发，给所有的派生链做一个定义。	
	void registerCircle(LOC StartLoc, LOC NextLoc);//从一个格出发，确定是否为一个环。	
	void searchingFromBox(LOC BoxLoc);							
	//从一个格出发，注册他的所有派生链,ChianPlus应仅在没有短链时启用。
	void searchingCircle(LOC BoxLoc);//从一个格出发，注册他的所有派生环。
	void defineBoxesType();//将所有格子的基本信息填入，包括属主和格子类型   
	void defineAllChains(bool ChainPlus);//定义所有的链     				
    void resetChainsInfo();//重定义链与格的的信息  
	void defineDeadChain();//定义死环或者死链 
	void searchingDeadChain(LOC BoxLoc);//
	void registerDeadChain(LOC FreeBoxLoc, LOC FirstLoc);//注册死环或者死链 
	//说明：链是动态注册的。当一条链被归类到另一条链时候，他本身会被重新定义为"NotDefine"，而每次需要注册链的时候会从0开始查找到一条空链。
};

