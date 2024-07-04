#pragma once

#include <utility>
#include <vector>

struct FoodComponent {
    std::vector<std::pair<int,int>> m_foodPosition;
};

struct SnakeComponent {
    std::vector<std::pair<int, int>> m_position;
    double m_speed;
};