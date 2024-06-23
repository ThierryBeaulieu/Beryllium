#pragma once

#include <utility>
#include <vector>

class GameManager
{
public:
    GameManager(int gridWidth, int gridHeight);

    std::vector<std::pair<int, int>> GetFoodPosition();
    std::vector<std::pair<int, int>> GetSnakePosition();

private:
    int m_gridWidth;
    int m_gridHeight;

    std::vector<std::pair<int, int>> m_foodPosition;
    std::vector<std::pair<int, int>> m_snakePosition;
};

