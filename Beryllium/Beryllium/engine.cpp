#include "engine.h"

Engine::Engine(size_t imageWidth, size_t imageHeight, uint32_t* imageData)
    : m_imageWidth(imageWidth)
    , m_imageHeight(imageHeight)
    , m_imageData(imageData)
    , m_gameManager(GameManager(30, 30))
{

}

void Engine::Render() {

    

    std::vector<std::pair<int, int>> foodPosition = m_gameManager.GetFoodPosition();

    for (std::pair<int, int> foodCoord : foodPosition) {

    }


    std::vector<std::pair<int, int>> snakePosition = m_gameManager.GetSnakePosition();
    
}

void Engine::Update() {
    m_gameManager.Update();
}

void Engine::SetPixel(int x, int y, uint32_t color) {
    m_imageData[m_imageWidth * y + x] = color;
}

uint32_t Engine::GetPixel(int x, int y) {
    return m_imageData[m_imageWidth * y + x];
}


