#pragma once

#include <cstdlib>
#include <time.h>
#include <random>
#include <utility>
#include <vector>
#include <list>

#include "imgui.h"

enum class NextDirection
{
    Right,
    Left,
    Up,
    Down,
    None
};

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
    void HandleInput();

    void InitializeFood();
    void InitializeSnake();

    void GenerateFood();

    void SetDirectionUp();
    void SetDirectionDown();
    void SetDirectionLeft();
    void SetDirectionRight();

private:
    int m_gridWidth;
    int m_gridHeight;

    std::list<std::pair<int, int>> m_foodPosition;
    std::list<std::pair<int, int>> m_snakePosition;

    bool m_isGameOver;
    bool m_isGameWon;
    bool m_isGameBeginning;
    NextDirection m_nextDirection;
};