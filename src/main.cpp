#include "./AI/UCT.h"
#include "./AI/board.h"
#include "./AI/define.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <iostream>
Board *gameBoard;
sf::Font font;
sf::Clock gameClock;
int nowPlayer = BLACK;
bool GameBegin = false;
bool HumanFirst = true;
sf::Time X_Time = sf::seconds(0.f);
sf::Time H_Time = sf::seconds(0.f);
void DrawCircle(sf::RenderWindow &Window)
{
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            sf::CircleShape c(15);
            c.setFillColor(sf::Color::Black);
            c.setPosition(110 + i * 150, 110 + j * 150);
            Window.draw(c);
        }
    }
}
void ShowVisualLine(sf::RenderWindow &Window)
{
    sf::RectangleShape line(sf::Vector2f(120.f, 10.f));
    sf::Vector2i pos = sf::Mouse::getPosition(Window);
    int x = pos.x;
    int y = pos.y;
    line.setFillColor(sf::Color(97, 97, 97));
    if (x < 890 && x > 110 && y < 890 && y > 110)
    {
        if ((y - 110) % 150 < 30 && (x - 140) % 150 < 120)
        {
            line.setPosition((x - 110) / 150 * 150 + 140, (y - 110) / 150 * 150 + 120);
            Window.draw(line);
        }
        else if ((x - 110) % 150 < 30 && (y - 140) % 150 < 120)
        {
            line.rotate(90.f);
            line.setPosition((x - 110) / 150 * 150 + 130, (y - 110) / 150 * 150 + 140);
            Window.draw(line);
        }
    }
}
void ShowLine(sf::RenderWindow &Window)
{
    for (int i = 0; i < 11; i += 2)
    {
        for (int j = 1; j < 11; j += 2)
        {
            if (gameBoard->map[i][j] == OCCLINE)
            {
                sf::RectangleShape line(sf::Vector2f(120.f, 10.f));
                line.setFillColor(sf::Color::Black);
                line.setPosition(j / 2 * 150 + 140, i / 2 * 150 + 120);
                Window.draw(line);
            }
        }
    }
    for (int i = 1; i < 11; i += 2)
    {
        for (int j = 0; j < 11; j += 2)
        {
            if (gameBoard->map[i][j] == OCCLINE)
            {
                sf::RectangleShape line(sf::Vector2f(10.f, 120.f));
                line.setFillColor(sf::Color::Black);
                // 棋盘的x对应y，棋盘的y对应x
                line.setPosition(j / 2 * 150 + 120, i / 2 * 150 + 140);
                Window.draw(line);
            }
        }
    }
}
void ShowBox(sf::RenderWindow &Window)
{
    for (int i = 1; i < 11; i += 2)
    {
        for (int j = 1; j < 11; j += 2)
        {
            if (gameBoard->map[i][j] != EMPTY)
            {
                sf::RectangleShape box(sf::Vector2f(120.f, 120.f));
                if (gameBoard->map[i][j] == BLACK)
                {
                    box.setFillColor(sf::Color(244, 67, 54));
                }
                else
                {
                    box.setFillColor(sf::Color(33, 150, 243));
                }
                box.setPosition(j / 2 * 150 + 140, i / 2 * 150 + 140);
                Window.draw(box);
            }
        }
    }
}
void SideBar(sf::RenderWindow &Window)
{
    ///////先后手////////
    sf::Text x_title(L"先手", font, 50);
    sf::Text h_title(L"后手", font, 50);
    x_title.setPosition(1100, 25);
    h_title.setPosition(1300, 25);
    Window.draw(x_title);
    Window.draw(h_title);
    ////////得分////////
    sf::Text x_score(std::to_string(gameBoard->blackBox), font, 40);
    sf::Text h_score(std::to_string(gameBoard->whiteBox), font, 40);
    x_score.setPosition(1130, 80);
    h_score.setPosition(1330, 80);
    Window.draw(x_score);
    Window.draw(h_score);
    /////////开始游戏////////
    sf::Text BeginText("", font, 50);
    sf::RectangleShape GameButton(sf::Vector2f(300.f, 100.f));
    GameButton.setPosition(1100, 350);
    BeginText.setPosition(1150, 370);
    if (!GameBegin)
    {
        BeginText.setString(L"开始游戏");
        GameButton.setFillColor(sf::Color(76, 175, 80));
    }
    else
    {
        BeginText.setString(L"结束游戏");
        GameButton.setFillColor(sf::Color(183, 28, 28));
    }
    Window.draw(GameButton);
    Window.draw(BeginText);
    ////////时间//////////
    sf::Text xtt("", font, 30);
    sf::Text htt("", font, 30);
    int xt = 0, ht = 0;
    if (GameBegin)
    {
        if (nowPlayer == BLACK)
        {
            xt = X_Time.asSeconds() + gameClock.getElapsedTime().asSeconds();
            ht = H_Time.asSeconds();
        }
        else
        {
            xt = X_Time.asSeconds();
            ht = H_Time.asSeconds() + gameClock.getElapsedTime().asSeconds();
        }
    }
    if (xt > 60)
    {
        xtt.setPosition(1100, 150);
        string t;
        t += std::to_string(xt / 60);
        t += "m";
        t += std::to_string(xt % 60);
        t += "s";
        xtt.setString(t);
    }
    else
    {
        xtt.setPosition(1135, 150);
        xtt.setString(std::to_string(xt) + "s");
    }
    if (ht > 60)
    {
        htt.setPosition(1300, 150);
        string t;
        t += std::to_string(ht / 60);
        t += "m";
        t += std::to_string(ht % 60);
        t += "s";
        htt.setString(t);
    }
    else
    {
        htt.setPosition(1335, 150);
        htt.setString(std::to_string(ht) + "s");
    }
    Window.draw(xtt);
    Window.draw(htt);
    ////////先后手交换/////////
    sf::Text WhoFirst("", font, 30);
    sf::RectangleShape FirstButton(sf::Vector2f(150.f, 80.f));
    FirstButton.setPosition(1175, 250);
    if (HumanFirst)
    {
        WhoFirst.setPosition(1190, 270);
        FirstButton.setFillColor(sf::Color(255, 152, 0));
        WhoFirst.setString(L"玩家优先");
    }
    else
    {
        WhoFirst.setPosition(1205, 270);
        FirstButton.setFillColor(sf::Color(0, 188, 212));
        WhoFirst.setString(L"AI优先");
    }
    Window.draw(FirstButton);
    Window.draw(WhoFirst);
}
int main()
{
    gameBoard = new Board;
    sf::RenderWindow mainWindow;
    sf::Texture wooden;
    wooden.loadFromFile("res/board.jpg", sf::IntRect(0, 0, 1000, 1000));
    sf::Sprite board(wooden);

    font.loadFromFile("res/LXGWWenKai-Bold.ttf");

    mainWindow.create(sf::VideoMode(1500, 1000), L"Heap Overflow 点格棋");
    mainWindow.setVerticalSyncEnabled(true); // 垂直同步
    while (mainWindow.isOpen())
    {
        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                mainWindow.close();
                break;
            case sf::Event::MouseButtonPressed:
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                ////////占边////////////
                if (x < 890 && x > 110 && y < 890 && y > 110)
                {
                    if ((y - 110) % 150 < 30 && (x - 140) % 150 < 120)
                    {
                        // x坐标对应棋盘的y，y坐标对应棋盘的x
                        int bx = (y - 110) / 150 * 2;
                        int by = (x - 140) / 150 * 2 + 1;
                        if (gameBoard->map[bx][by] == OCCLINE)
                        {
                            break;
                        }
                        if (gameBoard->move(nowPlayer, {bx, by}) == 0)
                        {
                            nowPlayer = -nowPlayer;
                        }
                    }
                    else if ((x - 110) % 150 < 30 && (y - 140) % 150 < 120)
                    {
                        // x坐标对应棋盘的y，y坐标对应棋盘的x
                        int bx = (y - 140) / 150 * 2 + 1;
                        int by = (x - 110) / 150 * 2;
                        if (gameBoard->map[bx][by] == OCCLINE)
                        {
                            break;
                        }
                        if (gameBoard->move(nowPlayer, {bx, by}) == 0)
                        {
                            nowPlayer = -nowPlayer;
                        }
                    }
                }
                ////////开始游戏/////////
                else if (x < 1400 && x > 1100 && y > 350 && y < 450)
                {
                    if (GameBegin)
                    {
                        GameBegin = false;
                        delete gameBoard;
                        gameBoard = new Board;
                        nowPlayer = BLACK;
                    }
                    else
                    {
                        X_Time = H_Time = sf::seconds(0.f);
                        if (nowPlayer == BLACK)
                        {
                            if (!HumanFirst)
                            {
                                UCT(gameBoard, nowPlayer);
                            }
                        }
                        else
                        {
                            if (HumanFirst)
                            {
                                UCT(gameBoard, nowPlayer);
                            }
                        }
                        gameClock.restart();
                        GameBegin = true;
                    }
                }
                /////////谁优先//////////
                else if (x > 1175 && x < 1325 && y > 250 && y < 330)
                {
                    HumanFirst ^= 1;
                }
                break;
            }
        }
        if (GameBegin)
        {
            if (nowPlayer == BLACK)
            {
                if (!HumanFirst)
                {
                    H_Time += gameClock.getElapsedTime();
                    gameClock.restart();
                    UCT(gameBoard, nowPlayer);
                    nowPlayer = -nowPlayer;
                    X_Time += gameClock.getElapsedTime();
                    gameClock.restart();
                }
            }
            else
            {
                if (HumanFirst)
                {
                    X_Time += gameClock.getElapsedTime();
                    gameClock.restart();
                    UCT(gameBoard, nowPlayer);
                    nowPlayer = -nowPlayer;
                    H_Time += gameClock.getElapsedTime();
                    gameClock.restart();
                }
            }
        }
        mainWindow.clear(sf::Color(66, 66, 66));
        mainWindow.draw(board);
        DrawCircle(mainWindow);
        ShowVisualLine(mainWindow);
        ShowLine(mainWindow);
        ShowBox(mainWindow);
        SideBar(mainWindow);
        mainWindow.display();
    }
    return 0;
}