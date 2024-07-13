#include "gamemanager.h"

GameManager::GameManager()
    : m_GridWidth(0), m_GridHeight(0), m_CurrentDirection(Direction::None), m_GameState(GameState::MainMenu), m_SendForm(nullptr)
{
    std::thread thread_obj(&GameManager::PlayBackgroundSound, this);
    thread_obj.detach();
    srand(time(0));
}

std::list<std::pair<int, int>> GameManager::GetFoodPosition()
{
    return m_FoodPosition;
}

std::list<std::pair<int, int>> GameManager::GetSnakePosition()
{
    return m_SnakePosition;
}

int GameManager::GetGridWidth()
{
    return m_GridWidth;
}

void GameManager::SetGridWidth(int gridWidth)
{
    m_GridWidth = gridWidth;
}

int GameManager::GetGridHeight()
{
    return m_GridHeight;
}

void GameManager::SetGridHeight(int gridHeight)
{
    m_GridHeight = gridHeight;
}

void GameManager::Update()
{
    if (m_GameState == GameState::MainMenu)
    {
        UIManager &uiManager = UIManager::GetInstance();
        bool buttonState = uiManager.DisplayUI(UI::MainMenu);
        if (buttonState)
        {
            StartGame();
        }
    }

    if (m_SnakePosition.size() == m_GridHeight * m_GridWidth)
    {
        m_GameState = GameState::Victory;
    }

    if (m_GameState == GameState::Victory)
    {
        // todo: display ui
        return;
    }

    if (m_GameState == GameState::Over)
    {
        if (m_SnakePosition.size() > 0 || m_FoodPosition.size() > 0)
        {
            m_SnakePosition.clear();
            m_FoodPosition.clear();
        }

        UIManager &uiManager = UIManager::GetInstance();

        if (m_SendForm != nullptr && *m_SendForm)
        {
            ScoreManager &scoreManager = ScoreManager::GetInstance();
            UserFormData data = uiManager.GetDataForm();
            scoreManager.AddScore(Score(data.lastName, data.firstName, scoreManager.GetLastGameScore()));
            uiManager.RemoveUI(UI::AskingForPlayerName);
        }

        bool buttonState = uiManager.DisplayUI(UI::GameOver);
        if (buttonState)
        {
            uiManager.RemoveUI(UI::AskingForPlayerName);
            uiManager.RemoveUI(UI::GameOver);
            m_GameState = GameState::Playing;
            ResetGame();
            StartGame();
        }

        return;
    }

    InitGame();
    HandleInput();

    std::pair<int, int> foodPosition = m_FoodPosition.front();
    const std::pair<int, int> &snakeHead = m_SnakePosition.front();

    for (const std::pair<int, int> &snakePosition : m_SnakePosition)
    {
        if (snakePosition == foodPosition)
        {
            std::thread thread_obj(&GameManager::PlayUpgradeSound, this);
            thread_obj.detach();

            ScoreManager &scoreManager = ScoreManager::GetInstance();
            scoreManager.IncrementPlayerScore();

            m_FoodPosition.pop_back();
            m_SnakePosition.push_back(foodPosition);
            break;
        }
        if (snakePosition == snakeHead && &snakePosition != &snakeHead)
        {
            m_GameState = GameState::Over;
            std::thread thread_obj(&GameManager::PlayGameOverSound, this);
            thread_obj.detach();
            break;
        }
    }
}

void GameManager::HandleDeath()
{
    m_GameState = GameState::Over;
    std::thread thread_obj(&GameManager::PlayGameOverSound, this);
    thread_obj.detach();

    ScoreManager &scoreManager = ScoreManager::GetInstance();
    scoreManager.ResetPlayerScore();

    UIManager &uiManager = UIManager::GetInstance();
    m_SendForm = &uiManager.DisplayUI(UI::AskingForPlayerName);
}

void GameManager::HandleInput()
{
    if (m_SnakePosition.size() == 0)
        return;

    std::pair<int, int> front = m_SnakePosition.front();

    if (m_GameState == GameState::MainMenu)
        return;

    switch (m_CurrentDirection)
    {
    case Direction::Up:
        if (front.second <= 0)
        {
            HandleDeath();
        }

        if (front.second - 1 < 0)
            return;

        m_SnakePosition.pop_back();
        m_SnakePosition.push_front(std::make_pair(front.first, front.second - 1));
        break;

    case Direction::Down:
        if (front.second >= m_GridHeight - 1)
        {
            HandleDeath();
        }

        if (front.second + 1 >= m_GridHeight)
            return;

        m_SnakePosition.pop_back();
        m_SnakePosition.push_front(std::make_pair(front.first, front.second + 1));
        break;

    case Direction::Left:
        if (front.first <= 0)
        {
            HandleDeath();
        }

        if (front.first - 1 < 0)
            return;

        m_SnakePosition.pop_back();
        m_SnakePosition.push_front(std::make_pair(front.first - 1, front.second));
        break;

    case Direction::Right:
        if (front.first >= m_GridWidth - 1)
        {
            HandleDeath();
        }

        if (front.first + 1 >= m_GridWidth)
            return;

        m_SnakePosition.pop_back();
        m_SnakePosition.push_front(std::make_pair(front.first + 1, front.second));
        break;

    default:
        break;
    }
}

void GameManager::InitializeFood()
{
    int foodInitWidth = std::rand() % m_GridWidth;
    int foodInitHeight = std::rand() % m_GridHeight;

    m_FoodPosition.push_back(std::make_pair(foodInitWidth, foodInitHeight));
}

void GameManager::InitializeSnake()
{
    int snakeInitWidth = std::rand() % m_GridWidth;
    int snakeInitHeight = std::rand() % m_GridHeight;

    std::pair<int, int> foodPos = m_FoodPosition.front();
    while (foodPos == std::make_pair(snakeInitWidth, snakeInitHeight))
    {
        snakeInitWidth = std::rand() % m_GridWidth;
        snakeInitHeight = std::rand() % m_GridHeight;
    }

    m_SnakePosition.push_back(std::make_pair(snakeInitWidth, snakeInitHeight));
}

void GameManager::GenerateFood()
{
    const int foodInitWidth = std::rand() % m_GridWidth;
    const int foodInitHeight = std::rand() % m_GridHeight;
    m_FoodPosition.push_back(std::make_pair(foodInitWidth, foodInitHeight));
}

void GameManager::StartGame()
{
    if (m_FoodPosition.size() == 0)
        InitializeFood();

    if (m_SnakePosition.size() == 0)
        InitializeSnake();

    m_GameState = GameState::Playing;

    UIManager &uiManager = UIManager::GetInstance();
    uiManager.RemoveUI(UI::MainMenu);
    uiManager.RemoveUI(UI::GameOver);
    SetInitDirection();
}

void GameManager::InitGame()
{
    if (m_FoodPosition.size() == 0)
        InitializeFood();

    if (m_SnakePosition.size() == 0)
        InitializeSnake();
}

void GameManager::SetInitDirection()
{
    if (m_SnakePosition.size() <= 0)
        return;

    std::pair<int, int> snakePosition = m_SnakePosition.front();
    if (snakePosition.second > m_GridHeight / 2)
    {
        m_CurrentDirection = Direction::Up;
    }
    else
    {
        m_CurrentDirection = Direction::Down;
    }
}

void GameManager::ResetGame()
{
    m_GameState = GameState::MainMenu;
    m_CurrentDirection = Direction::None;
    m_FoodPosition.clear();
    m_SnakePosition.clear();
}

void GameManager::SetDirectionUp()
{
    if (m_CurrentDirection == Direction::Down && m_SnakePosition.size() > 1)
    {
        return;
    }
    m_CurrentDirection = Direction::Up;
}

void GameManager::SetDirectionDown()
{
    if (m_CurrentDirection == Direction::Up && m_SnakePosition.size() > 1)
    {
        return;
    }
    m_CurrentDirection = Direction::Down;
}

void GameManager::SetDirectionLeft()
{
    if (m_CurrentDirection == Direction::Right && m_SnakePosition.size() > 1)
    {
        return;
    }
    m_CurrentDirection = Direction::Left;
}

void GameManager::SetDirectionRight()
{
    if (m_CurrentDirection == Direction::Left && m_SnakePosition.size() > 1)
    {
        return;
    }
    m_CurrentDirection = Direction::Right;
}

void GameManager::PlayUpgradeSound()
{
    SoundDevice *soundDevice = SoundDevice::get();
    uint32_t sound1 = SoundBuffer::get()->AddSoundEffect("sounds/power_up.wav");
    SoundSource mySpeaker;
    mySpeaker.Play(sound1);
}

void GameManager::PlayGameOverSound()
{
    SoundDevice *soundDevice = SoundDevice::get();
    uint32_t sound1 = SoundBuffer::get()->AddSoundEffect("sounds/death.wav");
    SoundSource mySpeaker;
    mySpeaker.Play(sound1);
}

void GameManager::PlayBackgroundSound()
{
    SoundDevice *soundDevice = SoundDevice::get();
    uint32_t sound1 = SoundBuffer::get()->AddSoundEffect("sounds/danger.wav");
    SoundSource mySpeaker;
    mySpeaker.PlayLoop(sound1);
}