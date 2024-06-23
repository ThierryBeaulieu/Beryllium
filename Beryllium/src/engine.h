#pragma once

#include <chrono>
#include <cstdint>
#include <GLFW/glfw3.h>
#include <vector>

#include "gamemanager.h"

class Engine
{
public:
    Engine(size_t imageWidth, size_t imageHeight, uint32_t *imageData, GLFWwindow *m_window);

    void Render();
    void Update();

    void HandleInput();

    void RenderSquare(int x, int y, uint32_t color);

    void SetPixel(int x, int y, uint32_t color);
    uint32_t GetPixel(int x, int y);

    static uint32_t GetColorWhite(uint8_t brightness)
    {
        return (brightness | brightness << 8 | brightness << 16 | brightness << 24);
    }

    static uint32_t GetColorRed(uint8_t brightness)
    {
        return (255 | 0 << 8 | 0 << 16 | brightness << 24);
    }

    GLFWwindow *m_window;
    GameManager m_gameManager;
    size_t m_imageWidth;
    size_t m_imageHeight;
    uint32_t *m_imageData;
    int m_pixelWidth;
    int m_paddingWidth;
};
