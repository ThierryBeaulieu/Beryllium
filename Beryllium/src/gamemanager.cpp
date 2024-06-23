#include "gamemanager.h"

GameManager::GameManager(int gridWidth, int gridHeight)
    : m_gridWidth(gridWidth)
    , m_gridHeight(gridHeight)
{
    srand(time(0));
}

std::vector<std::pair<int, int>> GameManager::GetFoodPosition() {
    return m_foodPosition;
}

std::vector<std::pair<int, int>> GameManager::GetSnakePosition() {
    return m_snakePosition;
}

int GameManager::GetGridWidth() {
    return m_gridWidth;
}

int GameManager::GetGridHeight() {
    return m_gridHeight;
}

void GameManager::Update() {
    if (m_foodPosition.size() == 0 || m_snakePosition.size() == 0)
        Initialize();
}

void GameManager::Initialize() {
    int foodInitWidth = std::rand() % m_gridWidth;
    int foodInitHeight = std::rand() % m_gridHeight;
    m_foodPosition.push_back(std::make_pair(300, 300));

    int snakeInitWidth = std::rand() % m_gridWidth;
    int snakeInitHeight = std::rand() % m_gridHeight;
    m_snakePosition.push_back(std::make_pair(600, 600));
}