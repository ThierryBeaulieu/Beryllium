#pragma once

#include <cstdlib>
#include <time.h>
#include <random>
#include <utility>
#include <vector>
#include <list>

class GameManager
{
public:
    GameManager();

    std::list<std::pair<int, int>> GetFoodPosition();
    std::list<std::pair<int, int>> GetSnakePosition();

    int GetGridWidth();
    int GetGridHeight();

    void SetGridWidth(int gridWidth);
    void SetGridHeight(int gridHeight);

    void Update();

    void Initialize();

    void SetDirectionUp();
    void SetDirectionDown();
    void SetDirectionLeft();
    void SetDirectionRight();

private:
    int m_gridWidth;
    int m_gridHeight;

    std::list<std::pair<int, int>> m_foodPosition;
    std::list<std::pair<int, int>> m_snakePosition;
};
