#include <SFML/Graphics.hpp>
#include "TrafficLight.h"
#include "GuiManager.h"

void TrafficLight::UpdatePosition(sf::Vector2f center, float intersection_size, int direction)
{
    if (!active)
        return;

    float offset = intersection_size / 2.f;
    float size = intersection_size / 3.f;
    float thickness = 5.f;

    switch (direction)
    {
    case 0:
        box.position = {center.x - offset, center.y - offset - thickness};
        box.size = {size, thickness};
        break;
    case 1:
        box.position = {center.x + offset + thickness, center.y - offset};
        box.size = {-thickness, size};
        break;
    case 2:
        box.position = {center.x + offset, center.y + offset + thickness};
        box.size = {-size, -thickness};
        break;
    case 3:
        box.position = {center.x - offset - thickness, center.y + offset};
        box.size = {thickness, -size};
        break;
    }
}

void TrafficLight::Draw()
{
    if (!active)
        return;

    sf::CircleShape lightShape;
    lightShape.setPosition(box.position);
    lightShape.setRadius(4.f);
    lightShape.setPointCount(8);

    if (color == Color::Green)
        lightShape.setFillColor(sf::Color::Green);
    else if (color == Color::Yellow)
        lightShape.setFillColor(sf::Color::Yellow);
    else
        lightShape.setFillColor(sf::Color::Red);

    GuiManager::getInstance()->window.draw(lightShape);

    if (GuiManager::getInstance()->draw_debug)
    {
        sf::RectangleShape boundingRect;
        boundingRect.setPosition(box.position);
        boundingRect.setSize(box.size);
        boundingRect.setFillColor(sf::Color::Transparent);
        boundingRect.setOutlineColor(sf::Color::White);
        boundingRect.setOutlineThickness(1.0f);
        GuiManager::getInstance()->window.draw(boundingRect);
    }
}
