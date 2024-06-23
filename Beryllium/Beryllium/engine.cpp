#include "engine.h"

Engine::Engine(size_t imageWidth, size_t imageHeight, uint32_t* imageData)
    : m_imageWidth(imageWidth)
    , m_imageHeight(imageHeight)
    , m_imageData(imageData)
{

}

void Engine::Render() {
}

void Engine::Update() {}

void Engine::SetPixel(int x, int y, uint32_t color) {
    m_imageData[m_imageWidth * y + x] = color;
}

uint32_t Engine::GetPixel(int x, int y) {
    return m_imageData[m_imageWidth * y + x];
}


