#include <cstdint>

#pragma once

class SnakeEngine
{
public:
    SnakeEngine(size_t imageWidth, size_t imageHeight, uint32_t* imageData);

    void Render();
    void Update();
private:
    size_t m_imageWidth;
    size_t m_imageHeight;
    uint32_t* m_imageData;
};