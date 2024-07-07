#include "gamemanager.h"

GameManager::GameManager()
    : m_gridWidth(0), m_gridHeight(0), m_currentDirection(Direction::None), m_gameState(GameState::MainMenu)
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
    m_userInterfaces.clear();
    /*if (m_gameState == GameState::MainMenu)
    {
        //UserInterface mainMenu {"Main Menu", g_imageWidth/2, g_imageHeight, 200.0f, 200.0f};
        //m_userInterfaces.push_back(mainMenu);
        return;
    }
     */

    if (m_snakePosition.size() == m_gridHeight * m_gridWidth)
    {
        m_gameState = GameState::Victory;
    }

    if (m_gameState == GameState::Victory)
    {
        // todo: display ui
        return;
    }

    if (m_gameState == GameState::Over)
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
            std::thread thread_obj(&GameManager::PlayUpgradeSound, this);
            thread_obj.detach();

            m_foodPosition.pop_back();
            m_snakePosition.push_back(foodPosition);
            break;
        }
        if (snakePosition == snakeHead && &snakePosition != &snakeHead)
        {
            m_gameState = GameState::Over;
            std::thread thread_obj(&GameManager::PlayGameOverSound, this);
            thread_obj.detach();
            break;
        }
    }
}

void GameManager::HandleInput()
{
    if (m_snakePosition.size() == 0)
        return;

    std::pair<int, int> front = m_snakePosition.front();

    switch (m_currentDirection)
    {
    case Direction::Up:
        if (front.second <= 0)
        {
            m_gameState = GameState::Over;
            std::thread thread_obj(&GameManager::PlayGameOverSound, this);
            thread_obj.detach();
        }

        if (front.second - 1 < 0)
            return;

        m_snakePosition.pop_back();
        m_snakePosition.push_front(std::make_pair(front.first, front.second - 1));
        break;

    case Direction::Down:
        if (front.second >= m_gridHeight - 1)
        {
            m_gameState = GameState::Over;
            std::thread thread_obj(&GameManager::PlayGameOverSound, this);
            thread_obj.detach();
        }

        if (front.second + 1 >= m_gridHeight)
            return;

        m_snakePosition.pop_back();
        m_snakePosition.push_front(std::make_pair(front.first, front.second + 1));
        break;

    case Direction::Left:
        if (front.first <= 0)
        {
            m_gameState = GameState::Over;
            std::thread thread_obj(&GameManager::PlayGameOverSound, this);
            thread_obj.detach();
        }

        if (front.first - 1 < 0)
            return;

        m_snakePosition.pop_back();
        m_snakePosition.push_front(std::make_pair(front.first - 1, front.second));
        break;

    case Direction::Right:
        if (front.first >= m_gridWidth - 1)
        {
            m_gameState = GameState::Over;
            std::thread thread_obj(&GameManager::PlayGameOverSound, this);
            thread_obj.detach();
        }

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

void GameManager::StartGame()
{
    m_currentDirection = Direction::Down;
}

void GameManager::ResetGame()
{
    m_gameState = GameState::MainMenu;
    m_currentDirection = Direction::None;
    m_foodPosition.clear();
    m_snakePosition.clear();
}

void GameManager::SetDirectionUp()
{
    if (m_currentDirection == Direction::Down && m_snakePosition.size() > 1)
    {
        return;
    }
    m_currentDirection = Direction::Up;
}

void GameManager::SetDirectionDown()
{
    if (m_currentDirection == Direction::Up && m_snakePosition.size() > 1)
    {
        return;
    }
    m_currentDirection = Direction::Down;
}

void GameManager::SetDirectionLeft()
{
    if (m_currentDirection == Direction::Right && m_snakePosition.size() > 1)
    {
        return;
    }
    m_currentDirection = Direction::Left;
}

void GameManager::SetDirectionRight()
{
    if (m_currentDirection == Direction::Left && m_snakePosition.size() > 1)
    {
        return;
    }
    m_currentDirection = Direction::Right;
}

void GameManager::PlayUpgradeSound()
{
    SoundDevice *soundDevice = SoundDevice::get();
    uint32_t sound1 = SoundBuffer::get()->addSoundEffect("sounds/power_up.wav");
    SoundSource mySpeaker;
    mySpeaker.Play(sound1);
}

void GameManager::PlayGameOverSound()
{
    SoundDevice *soundDevice = SoundDevice::get();
    uint32_t sound1 = SoundBuffer::get()->addSoundEffect("sounds/death.wav");
    SoundSource mySpeaker;
    mySpeaker.Play(sound1);
}

const std::vector<UserInterface> &GameManager::GetUserInterfaces()
{
    return m_userInterfaces;
}
