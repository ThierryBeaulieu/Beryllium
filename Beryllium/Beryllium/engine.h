#pragma once

#include <cstdint>
#include <vector>
#include <GLFW/glfw3.h>

class Engine
{
public:
    Engine(size_t imageWidth, size_t imageHeight, uint32_t* imageData);

    void Render();
    void Update();

    void SetPixel(int x, int y, uint32_t color);
    uint32_t GetPixel(int x, int y);

    static uint32_t GetColor(uint8_t brightness) {
        return (brightness | brightness << 8 | brightness << 16 | brightness << 24);
    }

    size_t m_imageWidth;
    size_t m_imageHeight;
    uint32_t* m_imageData;
};
