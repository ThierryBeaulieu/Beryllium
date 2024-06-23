#include "gamemanager.h"

GameManager::GameManager(int gridWidth, int gridHeight)
    : m_gridWidth(gridWidth)
    , m_gridHeight(gridHeight)
{}

std::vector<std::pair<int, int>> GameManager::GetFoodPosition() {
    return m_foodPosition;
}
std::vector<std::pair<int, int>> GameManager::GetSnakePosition() {
    return m_snakePosition;
}