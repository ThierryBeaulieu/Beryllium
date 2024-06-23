#include "gamemanager.h"

GameManager::GameManager()
    : m_gridWidth(0), m_gridHeight(0)
{
    srand(time(0));
}

std::list<std::pair<int, int>> GameManager::GetFoodPosition()
{
    return m_foodPosition;
}

std::list<std::pair<int, int>> GameManager::GetSnakePosition()
{
    return m_snakePosition;
}

int GameManager::GetGridWidth()
{
    return m_gridWidth;
}

void GameManager::SetGridWidth(int gridWidth)
{
    m_gridWidth = gridWidth;
}

int GameManager::GetGridHeight()
{
    return m_gridHeight;
}

void GameManager::SetGridHeight(int gridHeight)
{
    m_gridHeight = gridHeight;
}

void GameManager::Update()
{
    if (m_foodPosition.size() == 0 || m_snakePosition.size() == 0)
        Initialize();
}

void GameManager::Initialize()
{
    int foodInitWidth = std::rand() % m_gridWidth;
    int foodInitHeight = std::rand() % m_gridHeight;
    m_foodPosition.push_back(std::make_pair(foodInitWidth, foodInitHeight));

    int snakeInitWidth = std::rand() % m_gridWidth;
    int snakeInitHeight = std::rand() % m_gridHeight;

    while (snakeInitWidth == foodInitWidth && snakeInitHeight == foodInitHeight)
    {
        snakeInitWidth = std::rand() % m_gridWidth;
        snakeInitHeight = std::rand() % m_gridHeight;
    }

    m_snakePosition.push_back(std::make_pair(snakeInitWidth, snakeInitHeight));
}

void GameManager::SetDirectionUp()
{
    std::pair<int, int> front = m_snakePosition.front();

    if (front.second - 1 < 0)
        return;

    m_snakePosition.pop_back();
    m_snakePosition.push_front(std::make_pair(front.first, front.second - 1));
}

void GameManager::SetDirectionDown()
{
    std::pair<int, int> front = m_snakePosition.front();

    if (front.second + 1 >= m_gridHeight)
        return;

    m_snakePosition.pop_back();
    m_snakePosition.push_front(std::make_pair(front.first, front.second + 1));
}

void GameManager::SetDirectionLeft()
{
    std::pair<int, int> front = m_snakePosition.front();

    if (front.first - 1 < 0)
        return;

    m_snakePosition.pop_back();
    m_snakePosition.push_front(std::make_pair(front.first - 1, front.second));
}

void GameManager::SetDirectionRight()
{
    std::pair<int, int> front = m_snakePosition.front();

    if (front.first + 1 >= m_gridWidth)
        return;

    m_snakePosition.pop_back();
    m_snakePosition.push_front(std::make_pair(front.first + 1, front.second));
}