#include "engine.h"

Engine::Engine(size_t imageWidth, size_t imageHeight, uint32_t *imageData)
    : m_imageWidth(imageWidth), m_imageHeight(imageHeight), m_imageData(imageData), m_gameManager(GameManager()), m_pixelWidth(30), m_paddingWidth(2)
{
    int nbPixelsWidth = (m_imageWidth - (m_imageWidth % m_pixelWidth)) / m_pixelWidth;
    int nbPixelsHeight = (m_imageHeight - (m_imageHeight % m_pixelWidth)) / m_pixelWidth;

    m_gameManager.SetGridHeight(nbPixelsHeight);
    m_gameManager.SetGridWidth(nbPixelsWidth);
}

void Engine::RenderSquare(int x, int y, uint32_t color)
{
    for (int i = 0; i < m_pixelWidth; ++i)
    {
        for (int j = 0; j < m_pixelWidth; ++j)
        {
            if (i < m_paddingWidth || i > (m_pixelWidth - m_paddingWidth) ||
                j < m_paddingWidth || j > (m_pixelWidth - m_paddingWidth))
                continue;
            SetPixel(x + i, y + j, color);
        }
    }
}

void Engine::Render()
{
    int nbPixelsWidth = (m_imageWidth - (m_imageWidth % m_pixelWidth)) / m_pixelWidth;
    int nbPixelsHeight = (m_imageHeight - (m_imageHeight % m_pixelWidth)) / m_pixelWidth;

    uint32_t tileColor = Engine::GetColorWhite(200);

    for (int i = 0; i < nbPixelsWidth; ++i)
    {
        for (int j = 0; j < nbPixelsHeight; ++j)
        {
            RenderSquare(i * m_pixelWidth, j * m_pixelWidth, tileColor);
        }
    }

    uint32_t foodColor = Engine::GetColorRed(255);
    for (std::pair<int, int> foodCoord : m_gameManager.GetFoodPosition())
    {
        RenderSquare(foodCoord.first * m_pixelWidth, foodCoord.second * m_pixelWidth, foodColor);
    }

    uint32_t snakeColor = Engine::GetColorWhite(255);
    for (std::pair<int, int> snakeCoord : m_gameManager.GetSnakePosition())
    {
        RenderSquare(snakeCoord.first * m_pixelWidth, snakeCoord.second * m_pixelWidth, snakeColor);
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
