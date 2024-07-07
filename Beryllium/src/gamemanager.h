#pragma once

#include <cstdlib>
#include <future>
#include <time.h>
#include <random>
#include <utility>
#include <vector>
#include <list>

#include "constants.h"
#include "imgui.h"
#include "soundbuffer.h"
#include "sounddevice.h"
#include "soundsource.h"
#include "uimanager.h"

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

struct UserInterface
{
    const char *name;
    int coord_x;
    int coord_y;
    int width;
    int height;
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
    void StartGame();

    // tbeaulieu2 : handle the input in a different way?
    void SetDirectionUp();
    void SetDirectionDown();
    void SetDirectionLeft();
    void SetDirectionRight();
    // tbeaulieu2

    // tbeaulieu2 : todo put in another place
    void PlayUpgradeSound();
    void PlayGameOverSound();
    void PlayBeginSound();
    // end todo

    void ResetGame();

private:
    // tbeaulieu2 todo: change the norma from m_gridWidth to m_GridWidth
    int m_gridWidth;
    int m_gridHeight;

    std::list<std::pair<int, int>> m_foodPosition;
    std::list<std::pair<int, int>> m_snakePosition;

    Direction m_currentDirection;
    GameState m_gameState;
    UIManager &uiManager = UIManager::GetInstance();
    // tbeaulieu 2 end;
};
