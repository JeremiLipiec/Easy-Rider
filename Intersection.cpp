#include <SFML/Graphics.hpp>
#include "Intersection.h"
#include "GuiManager.h"
#include <cstdlib>

using namespace std;

Intersection::Intersection(int _id, sf::Vector2f _position, bool _has_traffic_lights)
{
    id = _id;
    position = _position;
    has_traffic_lights = _has_traffic_lights;
    used = false;
    light_direction = 0;
    boundingBox.position = {position.x - intersection_size / 2, position.y - intersection_size / 2};
    boundingBox.size = {intersection_size, intersection_size};
    SpawnLightBoxes();
    srand(position.x * position.y * id * 123865128);
    _time = rand() % (120 - 1);
}

void Intersection::SpawnLightBoxes()
{
    float offset = intersection_size / 2.f;
    float size = intersection_size / 3.f;
    float thickness = 5.f;

    light_boxes[0].position = {position.x - offset, position.y - offset - thickness};
    light_boxes[0].size = {size, thickness};

    light_boxes[1].position = {position.x + offset + thickness, position.y - offset};
    light_boxes[1].size = {-thickness, size};

    light_boxes[2].position = {position.x + offset, position.y + offset + thickness};
    light_boxes[2].size = {-size, -thickness};

    light_boxes[3].position = {position.x - offset - thickness, position.y + offset};
    light_boxes[3].size = {thickness, -size};
}

void Intersection::Update()
{
    if (_time >= 120)
    {
        light_direction++;
        if (light_direction > 3)
        {
            light_direction = 0;
        }
        _time = 0;
    }
    _time++;
}

void Intersection::Draw()
{
    //return;
    // draw intersections
    sf::RectangleShape intersection_shape({intersection_size, intersection_size});
    intersection_shape.setPosition({position.x - intersection_size / 2, position.y - intersection_size / 2});

    if (used)
    {
        intersection_shape.setFillColor(sf::Color(90, 90, 90));
    }
    else
    {
        intersection_shape.setFillColor(sf::Color(20, 20, 20));
    }

    GuiManager::getInstance()->window.draw(intersection_shape);

    // GuiManager::getInstance()->DrawText(to_string(id), position);
    GuiManager::getInstance()->DrawText(to_string(light_direction), position);

    // sf::FloatRect boundingBox = intersection_shape.getGlobalBounds();
    sf::RectangleShape boundingRect;
    boundingRect.setPosition(boundingBox.position);
    boundingRect.setSize(boundingBox.size);
    boundingRect.setFillColor(sf::Color::Transparent);
    boundingRect.setOutlineColor(sf::Color::Cyan);
    boundingRect.setOutlineThickness(1.0f);
    GuiManager::getInstance()->window.draw(boundingRect);

    for (int i = 0; i < 4; i++)
    {
        // draw light circles
        sf::CircleShape lightShape;
        lightShape.setPosition(light_boxes[i].position);
        lightShape.setRadius(4.f);
        lightShape.setPointCount(8);
        if (light_direction == i)
            lightShape.setFillColor(sf::Color::Green);
        else
            lightShape.setFillColor(sf::Color::Red);
        GuiManager::getInstance()->window.draw(lightShape);

        sf::RectangleShape boundingRect;
        boundingRect.setPosition(light_boxes[i].position);
        boundingRect.setSize(light_boxes[i].size);
        boundingRect.setFillColor(sf::Color::Transparent);
        boundingRect.setOutlineColor(sf::Color::White);
        boundingRect.setOutlineThickness(1.0f);
        GuiManager::getInstance()->window.draw(boundingRect);
    }
}
