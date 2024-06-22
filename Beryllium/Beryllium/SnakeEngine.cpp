#include "snakeengine.h"

SnakeEngine::SnakeEngine(size_t imageWidth, size_t imageHeight, uint32_t* imageData)
    : m_imageWidth(imageWidth)
    , m_imageHeight(imageHeight)
    , m_imageData(imageData)
    {
        SetPixel(m_imageWidth >> 1, m_imageHeight >> 1, static_cast<uint8_t>(100000));
        Food food = Food(*this);
    }

void SnakeEngine::Render() {}

void SnakeEngine::Update() {}

void SnakeEngine::SetPixel(int x, int y, uint32_t color) {
    m_imageData[m_imageWidth * y + x] = color;
}

uint32_t SnakeEngine::GetPixel(int x, int y) {
    return m_imageData[m_imageWidth * y + x];
}

Food::Food(SnakeEngine& snakeEngine) {
    constexpr int width = 30;
    constexpr int height = 30;
    int initPosX = (int)snakeEngine.m_imageWidth >> 1;
    int initPosY = (int)snakeEngine.m_imageHeight >> 1;

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            snakeEngine.SetPixel(initPosX + i, initPosY + j, SnakeEngine::GetColor(255));
        }
    }
}