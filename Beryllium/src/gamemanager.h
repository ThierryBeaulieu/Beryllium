#pragma once

#include <cstdlib>
#include <future>
#include <time.h>
#include <random>
#include <utility>
#include <vector>
#include <list>

#include "imgui.h"
#include "soundbuffer.h"
#include "sounddevice.h"
#include "soundsource.h"

enum class Direction
{
    Right,
    Left,
    Up,
    Down,
    None
};

enum class GameState
{
    MainMenu,
    WaitingForGameStart,
    Playing,
    Victory,
    Pause,
    Over,
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


    void PlayUpgradeSound();
    void PlayGameOverSound();
    void PlayBeginSound();

private:
    int m_gridWidth;
    int m_gridHeight;

    std::list<std::pair<int, int>> m_foodPosition;
    std::list<std::pair<int, int>> m_snakePosition;

    Direction m_currentDirection;
    GameState m_gameState;
};
