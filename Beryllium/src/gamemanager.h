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


    static void GameManager::PlaySound() {
        SoundDevice* soundDevice = SoundDevice::get();
        uint32_t sound1 = SoundBuffer::get()->addSoundEffect("sounds/wood_impact.ogg");
        SoundSource mySpeaker;
        mySpeaker.Play(sound1);
    }

private:
    int m_gridWidth;
    int m_gridHeight;

    std::list<std::pair<int, int>> m_foodPosition;
    std::list<std::pair<int, int>> m_snakePosition;

    bool m_isGameOver;
    bool m_isGameWon;
    bool m_isGameBeginning;

    Direction m_currentDirection;
};
