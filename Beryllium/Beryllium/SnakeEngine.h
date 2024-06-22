#include <cstdint>
#include <vector>
#include <GLFW/glfw3.h>

#pragma once

class SnakeEngine
{
public:
    SnakeEngine(size_t imageWidth, size_t imageHeight, uint32_t* imageData);

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

class TileContent
{
public:
    TileContent(SnakeEngine& snakeEngine)
    : m_snakeEngine(snakeEngine)
    {}
    virtual void GetContent() = 0;
protected:
    SnakeEngine& m_snakeEngine;
};

class Food: TileContent
{
public:
    Food(SnakeEngine& snakeEngine)
    : TileContent(snakeEngine)
    {}

    void GetContent() override;
};

class Board
{
public:
    Board(int boardWidth, int boardHeight)
        : m_boardWidth(boardWidth)
        , m_boardHeight(boardHeight)
        {}
private:
    int m_boardWidth;
    int m_boardHeight;
    std::vector<TileContent> tiles;
};

