#pragma once

#include <chrono>
#include <cstdint>
#include <GLFW/glfw3.h>
#include <vector>

#include "constants.h"
#include "gamemanager.h"

class Engine
{
public:
    Engine(uint32_t *imageData, GLFWwindow *m_window);

    void Render(GameUI &gameState);
    void Update(GameUI &gameState, const std::chrono::duration<double> &elapsedTime);

private:
    void HandleInput();

    void RenderSquare(int x, int y, uint32_t color);
    void RenderBackground(uint32_t color);

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

    static uint32_t GetColorBlack(uint8_t brightness)
    {
        return (0 | 0 << 8 | 0 << 16 | brightness << 24);
    }

    GLFWwindow *m_window;
    GLuint m_imageTexture;

    uint32_t *m_imageData;
    int m_pixelWidth;
    int m_paddingWidth;
};
