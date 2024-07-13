#pragma once

#include <cstdlib>
#include <cstring>
#include <future>
#include <time.h>
#include <random>
#include <utility>
#include <vector>
#include <list>

#include "constants.h"
#include "imgui.h"
#include "scoremanager.h"
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

class GameManager
{
public:
    GameManager();

    // tbeaulieu2 todo:
    std::list<std::pair<int, int>> GetFoodPosition();
    std::list<std::pair<int, int>> GetSnakePosition();
    // tbeaulieu2 todo return reference

    int GetGridWidth();
    int GetGridHeight();

    void SetGridWidth(int gridWidth);
    void SetGridHeight(int gridHeight);

    void Update(const std::chrono::duration<double> &elapsedTime);
    void HandleInput();

    void InitializeFood();
    void InitializeSnake();

    void GenerateFood();
    void StartGame();
    void InitGame();
    void SetInitDirection();

    void HandleDeath();

    // tbeaulieu2 : handle the input in a different way?
    void SetDirectionUp();
    void SetDirectionDown();
    void SetDirectionLeft();
    void SetDirectionRight();
    // tbeaulieu2

    // tbeaulieu2 : todo put in another place
    void PlayUpgradeSound();
    void PlayBackgroundSound();
    void PlayGameOverSound();
    void PlayBeginSound();
    // end todo

    void ResetGame();

private:
    int m_GridWidth;
    int m_GridHeight;

    std::list<std::pair<int, int>> m_FoodPosition;
    std::list<std::pair<int, int>> m_SnakePosition;

    Direction m_CurrentDirection;
    GameState m_GameState;
    bool *m_SendForm;
    std::chrono::duration<double> m_TimeSinceLastMove{0.0};
    std::chrono::duration<double> m_SnakeSpeed{0.2};
};
