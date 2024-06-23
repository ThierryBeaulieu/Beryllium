#pragma once

#include <cstdlib>
#include <time.h>
#include <random>
#include <utility>
#include <vector>

class GameManager
{
public:
    GameManager();

    std::vector<std::pair<int, int>> GetFoodPosition();
    std::vector<std::pair<int, int>> GetSnakePosition();

    int GetGridWidth();
    int GetGridHeight();

    void SetGridWidth(int gridWidth);
    void SetGridHeight(int gridHeight);

    void Update();

    void Initialize();

private:
    int m_gridWidth;
    int m_gridHeight;

    std::vector<std::pair<int, int>> m_foodPosition;
    std::vector<std::pair<int, int>> m_snakePosition;
};
