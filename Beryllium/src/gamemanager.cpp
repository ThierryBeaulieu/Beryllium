#include "gamemanager.h"

GameManager::GameManager()
    : m_gridWidth(0), m_gridHeight(0), m_isGameOver(false), m_isGameWon(false), m_isGameBeginning(true), m_nextDirection(NextDirection::None)
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
    // if (m_isGameBeginning)
    // {
    //     // todo : display UI

    //     return;
    // }

    if (m_snakePosition.size() == m_gridHeight * m_gridWidth)
    {
        m_isGameWon = true;
    }

    if (m_isGameWon)
    {
        // todo: display ui
        return;
    }

    if (m_isGameOver)
    {
        if (m_snakePosition.size() > 0 || m_foodPosition.size() > 0)
        {
            m_snakePosition.clear();
            m_foodPosition.clear();
        }

        // todo : Display UI to start the game again

        return;
    }

    if (m_foodPosition.size() == 0)
        InitializeFood();

    if (m_snakePosition.size() == 0)
        InitializeSnake();

    HandleInput();

    std::pair<int, int> foodPosition = m_foodPosition.front();
    const std::pair<int, int> &snakeHead = m_snakePosition.front();

    for (const std::pair<int, int> &snakePosition : m_snakePosition)
    {
        if (snakePosition == foodPosition)
        {
            m_foodPosition.pop_back();
            m_snakePosition.push_back(foodPosition);
            break;
        }
        if (snakePosition == snakeHead && &snakePosition != &snakeHead)
        {
            m_isGameOver = true;
        }
    }

    // todo: handle the case when the snake goes backward.
    // todo: handle the snakes' speed
}

void GameManager::HandleInput()
{
    if (m_snakePosition.size() == 0)
        return;

    std::pair<int, int> front = m_snakePosition.front();

    if (front.second - 1 < 0)
        return;

    switch (m_nextDirection)
    {
    case NextDirection::Up:
        if (m_snakePosition.front().second <= 0)
            m_isGameOver = true;
        if (front.second - 1 < 0)
            return;

        m_snakePosition.pop_back();
        m_snakePosition.push_front(std::make_pair(front.first, front.second - 1));
        break;
    case NextDirection::Down:
        if (m_snakePosition.front().second >= m_gridHeight - 1)
            m_isGameOver = true;
        if (front.second + 1 >= m_gridHeight)
            return;

        m_snakePosition.pop_back();
        m_snakePosition.push_front(std::make_pair(front.first, front.second + 1));
        break;
    case NextDirection::Left:
        if (m_snakePosition.front().first <= 0)
            m_isGameOver = true;
        if (front.first - 1 < 0)
            return;

        m_snakePosition.pop_back();
        m_snakePosition.push_front(std::make_pair(front.first - 1, front.second));
        break;
    case NextDirection::Right:
        if (m_snakePosition.front().first >= m_gridWidth - 1)
            m_isGameOver = true;
        if (front.first + 1 >= m_gridWidth)
            return;

        m_snakePosition.pop_back();
        m_snakePosition.push_front(std::make_pair(front.first + 1, front.second));
        break;
    default:
        break;
    }
}

void GameManager::InitializeFood()
{
    int foodInitWidth = std::rand() % m_gridWidth;
    int foodInitHeight = std::rand() % m_gridHeight;

    m_foodPosition.push_back(std::make_pair(foodInitWidth, foodInitHeight));
}

void GameManager::InitializeSnake()
{
    int snakeInitWidth = std::rand() % m_gridWidth;
    int snakeInitHeight = std::rand() % m_gridHeight;

    std::pair<int, int> foodPos = m_foodPosition.front();
    while (foodPos == std::make_pair(snakeInitWidth, snakeInitHeight))
    {
        snakeInitWidth = std::rand() % m_gridWidth;
        snakeInitHeight = std::rand() % m_gridHeight;
    }

    m_snakePosition.push_back(std::make_pair(snakeInitWidth, snakeInitHeight));
}

void GameManager::GenerateFood()
{
    const int foodInitWidth = std::rand() % m_gridWidth;
    const int foodInitHeight = std::rand() % m_gridHeight;
    m_foodPosition.push_back(std::make_pair(foodInitWidth, foodInitHeight));
}

void GameManager::SetDirectionUp()
{
    m_nextDirection = NextDirection::Up;
}

void GameManager::SetDirectionDown()
{
    m_nextDirection = NextDirection::Down;
}

void GameManager::SetDirectionLeft()
{
    m_nextDirection = NextDirection::Left;
}

void GameManager::SetDirectionRight()
{
    m_nextDirection = NextDirection::Right;
}