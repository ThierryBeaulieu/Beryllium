#include "snakeengine.h"

SnakeEngine::SnakeEngine(size_t imageWidth, size_t imageHeight, uint32_t* imageData)
    : m_imageWidth(imageWidth)
    , m_imageHeight(imageHeight)
    , m_imageData(imageData)
    {
        SetPixel(m_imageWidth >> 1, m_imageHeight >> 1, SnakeEngine::MakeColor(255.0f));
    }

void SnakeEngine::Render() {}

void SnakeEngine::Update() {}

void SnakeEngine::SetPixel(int x, int y, uint32_t color) {
    m_imageData[m_imageWidth * y + x] = color;
}

uint32_t SnakeEngine::GetPixel(int x, int y) {
    return m_imageData[m_imageWidth * y + x];
}