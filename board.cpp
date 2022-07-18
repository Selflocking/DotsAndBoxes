#include "board.h"
#include <random>

/**
 * @brief 默认构造函数，分为DOT，BOX，HENG，SHU。
 * @details 同为偶是点，同为奇是格子，先偶后奇是横线，先奇后偶是竖线
 */
Board::Board()
{
    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            if (i % 2 == 0 && j % 2 == 0)
            {
                map[i][j] = DOT;
            }
            else if (i % 2 == 0 && j % 2 == 1)
            {
                map[i][j] = HENG;
            }
            else if (i % 2 == 1 && j % 2 == 1)
            {
                map[i][j] = BOX;
            }
            else
            {
                map[i][j] = SHU;
            }
        }
    }
    blackBox = whiteBox = 0;
}

/**
 * @brief 获得一个格子的自由度，自由度即格子周围未被占领的边的数量
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @return int 自由度
 */
int Board::getFreedom(int x, int y)
{
    int cnt = 4;
    if (map[x - 1][y] == OCCLINE)
        --cnt;
    if (map[x + 1][y] == OCCLINE)
        --cnt;
    if (map[x][y - 1] == OCCLINE)
        --cnt;
    if (map[x][y + 1] == OCCLINE)
        --cnt;
    return cnt;
}

/**
 * @brief 通过LOC返回自由度
 *
 * @param l 坐标
 * @return int 自由度
 */
int Board::getFreedom(LOC l)
{
    return getFreedom(l.first, l.second);
}


/**
 * @brief 占线
 * @details
 * 默认传进来的线都是没有被占过的，如果传进来的线是横线，要考虑它上下两个格子自由度是否为3；如果已经是3，那么再占一条线，这个格子就属于传进来下棋方的。同时要考虑边界情况，比如横线在最上面时，只需考虑线下面的格子，同理，如果是竖线，就要考虑左右两个格子
 *
 * @param owner 代表下棋方
 * @param l 代表要占的线的坐标
 * @return int 返回的是这条线产生的被占了的格子的数目
 */
int Board::move(int owner, LOC l)
{
    int res = 0;
    // 如果传进来的坐标不是边，输出
    //    if (!isFreeLine(l))
    //    {
    //        cerr << "Board::move(LOC l): " << l.first << " " << l.second << "\n";
    //        return 0;
    //    }
    if (map[l.first][l.second] == HENG)
    {
        if (l.first == 0)
        {
            if (getFreedom(1, l.second) == 1)
            {
                map[1][l.second] = owner;
                ++res;
            }
        }
        else if (l.first == 10)
        {
            if (getFreedom(9, l.second) == 1)
            {
                map[9][l.second] = owner;
                ++res;
            }
        }
        else
        {
            if (getFreedom(l.first - 1, l.second) == 1)
            {
                map[l.first - 1][l.second] = owner;
                ++res;
            }
            if (getFreedom(l.first + 1, l.second) == 1)
            {
                map[l.first + 1][l.second] = owner;
                ++res;
            }
        }
    }
    else
    {
        if (l.second == 0)
        {
            if (getFreedom(l.first, 1) == 1)
            {
                map[l.first][1] = owner;
                ++res;
            }
        }
        else if (l.second == 10)
        {
            if (getFreedom(l.first, 9) == 1)
            {
                map[l.first][9] = owner;
                ++res;
            }
        }
        else
        {
            if (getFreedom(l.first, l.second - 1) == 1)
            {
                map[l.first][l.second - 1] = owner;
                ++res;
            }
            if (getFreedom(l.first, l.second + 1) == 1)
            {
                map[l.first][l.second + 1] = owner;
                ++res;
            }
        }
    }
    map[l.first][l.second] = OCCLINE;
    if (owner == BLACK)
        blackBox += res;
    else
        whiteBox += res;
    return res;
}

/**
 * @brief 吃C型格，
 *
 * @param owner 代表此时的下棋方
 * @return LOC 返回一个C型格的坐标，找不到返回{-1，-1}
 */
LOC Board::eatCBox(int owner)
{
    for (int i = 1; i < 11; i += 2)
    {
        for (int j = 1; j < 11; j += 2)
        {
            if (getFreedom(i, j) == 1)
            {
                if (map[i - 1][j] != OCCLINE)
                {
                    move(owner, {i - 1, j});
                    return {i - 1, j};
                }
                else if (map[i + 1][j] != OCCLINE)
                {
                    move(owner, {i + 1, j});
                    return {i + 1, j};
                }
                else if (map[i][j + 1] != OCCLINE)
                {
                    move(owner, {i, j + 1});
                    return {i, j + 1};
                }
                else
                {
                    move(owner, {i, j - 1});
                    return {i, j - 1};
                }
            }
        }
    }
    // 找不到默认返回{-1，-1}
    return {-1, -1};
}

bool Board::isFreeLine(LOC l) const
{
    if (map[l.first][l.second] != SHU && map[l.first][l.second] != HENG)
        return false;
    else
        return true;
}

bool Board::isFreeLine(int i, int j) const
{
    if (map[i][j] != SHU && map[i][j] != HENG)
        return false;
    else
        return true;
}

/**
 * @brief 判断一个坐标是否会造成C型格
 *
 * @return ture表示会造成C型格
 */
bool Board::ifMakeCBox(LOC l)
{
    if (map[l.first][l.second] == HENG)
    {
        if (l.first == 0)
        {
            if (getFreedom(l.first, l.second + 1) == 2)
                return true;
            else
                return false;
        }
        else if (l.first == 10)
        {
            if (getFreedom(l.first, l.second - 1) == 2)
                return true;
            else
                return false;
        }
        else
        {
            if (getFreedom(l.first, l.second - 1) == 2 || getFreedom(l.first, l.second + 1) == 2)
                return true;
            else
                return false;
        }
    }
    else
    {
        if (l.second == 0)
        {
            if (getFreedom(l.first + 1, l.second) == 2)
                return true;
            else
                return false;
        }
        else if (l.second == 10)
        {
            if (getFreedom(l.first - 1, l.second) == 2)
                return true;
            else
                return false;
        }
        else
        {
            if (getFreedom(l.first - 1, l.second) == 2 || getFreedom(l.first + 1, l.second) == 2)
                return true;
            else
                return false;
        }
    }
}

/**
 * @brief 打印当前局面
 *
 * @details 按值显示棋面
 *
 * @todo 需要改进成更加直观的显示方法
 */
void Board::print()
{
    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 11; ++j)
        {
            printf("%2d ", map[i][j]);
        }
        cout << "\n";
    }
}

void Board::traverseEdge(std::function<void(int i, int j)> const &f)
{
    for (int i = 1; i < 11; i += 2)
    {
        for (int j = 0; j < 11; j += 2)
        {
            if (isFreeLine(i, j))
            {
                f(i, j);
            }
        }
    }
    for (int i = 0; i < 11; i += 2)
    {
        for (int j = 1; j < 11; j += 2)
        {
            if (isFreeLine(i, j))
            {
                f(i, j);
            }
        }
    }
}

void Board::traverseEdge(const std::function<void(LOC)> &f)
{
    for (int i = 1; i < 11; i += 2)
    {
        for (int j = 0; j < 11; j += 2)
        {
            if (isFreeLine(i, j))
            {
                f(LOC{i, j});
            }
        }
    }
    for (int i = 0; i < 11; i += 2)
    {
        for (int j = 1; j < 11; j += 2)
        {
            if (isFreeLine(i, j))
            {
                f(LOC{i, j});
            }
        }
    }
}

/**
 * @brief 判断是否赢得格子
 *
 * @param l 需要判断的边的坐标
 * @return 如果传进来的l占据了格子，返回true，否则返回false
 */
bool Board::ifEarnBox(LOC l)
{
    if (map[l.first][l.second] == HENG)
    {
        if (l.first == 0)
        {
            if (getFreedom(1, l.second) == 1)
            {
                return true;
            }
        }
        else if (l.first == 10)
        {
            if (getFreedom(9, l.second) == 1)
            {
                return true;
            }
        }
        else
        {
            if (getFreedom(l.first - 1, l.second) == 1)
            {
                return true;
            }
            if (getFreedom(l.first + 1, l.second) == 1)
            {
                return true;
            }
        }
    }
    else
    {
        if (l.second == 0)
        {
            if (getFreedom(l.first, 1) == 1)
            {
                return true;
            }
        }
        else if (l.second == 10)
        {
            if (getFreedom(l.first, 9) == 1)
            {
                return true;
            }
        }
        else
        {
            if (getFreedom(l.first, l.second - 1) == 1)
            {
                return true;
            }
            if (getFreedom(l.first, l.second + 1) == 1)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief 判断棋局是否结束
 *
 * @return 如果游戏已经结束，返回true，否则返回false
 */
bool Board::ifEnd() const
{
    if (blackBox > 12||whiteBox > 12)
        return true;
    else
        return false;
}

/**
 * @brief 判断赢家
 *
 * @return 如果黑方胜利，返回BLACK，白方胜利返回WHITE，目前无胜方返回EMPTY
 */
int Board::getWinner() const
{
       if (blackBox > 12)
           return BLACK;
       else if (whiteBox > 12)
           return WHITE;
       else
           return EMPTY;
}
