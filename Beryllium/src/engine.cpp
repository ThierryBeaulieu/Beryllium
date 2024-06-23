#include "engine.h"

Engine::Engine(size_t imageWidth, size_t imageHeight, uint32_t *imageData)
    : m_imageWidth(imageWidth), m_imageHeight(imageHeight), m_imageData(imageData), m_gameManager(GameManager(30, 30)), m_pixelWidth(30), m_paddingWidth(5)
{
}

void Engine::RenderSquare(int x, int y, uint32_t color)
{
    for (int i = 0; i < (m_pixelWidth + m_paddingWidth); ++i)
    {
        for (int j = 0; j < (m_pixelWidth + m_paddingWidth); ++j)
        {
            SetPixel(x + i, y + j, color);
        }
    }
}

void Engine::Render()
{
    uint32_t foodColor = Engine::GetColorRed(255);

    for (std::pair<int, int> foodCoord : m_gameManager.GetFoodPosition())
    {
        RenderSquare(foodCoord.first, foodCoord.second, foodColor);
    }

    uint32_t snakeColor = Engine::GetColorWhite(255);

    for (std::pair<int, int> snakeCoord : m_gameManager.GetSnakePosition())
    {
        RenderSquare(snakeCoord.first, snakeCoord.second, snakeColor);
    }
}

void Engine::Update()
{
    m_gameManager.Update();
}

void Engine::SetPixel(int x, int y, uint32_t color)
{
    m_imageData[m_imageWidth * y + x] = color;
}

uint32_t Engine::GetPixel(int x, int y)
{
    return m_imageData[m_imageWidth * y + x];
}
