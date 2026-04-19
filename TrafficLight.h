#pragma once
#include <SFML/Graphics.hpp>

class TrafficLight {
public:
    enum class Color { Red, Yellow, Green } color = Color::Red;

    bool active = false;
    sf::FloatRect box;

    void UpdatePosition(sf::Vector2f center, float intersection_size, int direction);
    void Draw();
};
