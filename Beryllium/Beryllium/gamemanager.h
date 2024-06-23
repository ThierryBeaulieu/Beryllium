#pragma once

#include <utility>
#include <vector>
#include <random>

class GameManager
{
public:
    GameManager(int gridWidth, int gridHeight);

    std::vector<std::pair<int, int>> GetFoodPosition();
    std::vector<std::pair<int, int>> GetSnakePosition();
    
    int GetGridWidth();
    int GetGridHeight();

    void Update();

    void Initialize();

private:
    int m_gridWidth;
    int m_gridHeight;

    std::vector<std::pair<int, int>> m_foodPosition;
    std::vector<std::pair<int, int>> m_snakePosition;
};

