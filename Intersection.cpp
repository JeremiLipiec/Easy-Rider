#include <SFML/Graphics.hpp>
#include "Intersection.h"
#include "GuiManager.h"
#include "Simulation.h"
#include <cstdlib>

using namespace std;

Intersection::Intersection(int _id, sf::Vector2f _local_position, bool _has_traffic_lights)
{
    id = _id;
    local_position = _local_position;
    has_traffic_lights = _has_traffic_lights;
    used = false;
    current_green_light_direction = 0;
    srand((int)(local_position.x * 73856093) ^ (int)(local_position.y * 19349663) ^ (id * 83492791));
    _time = rand() % (120 - 1);
    SpawnLightBoxes();
    UpdateGlobalPosition();
}

void Intersection::UpdateGlobalPosition()
{
    position = local_position + Simulation::getInstance()->infrastructure.drawing_origin;

    float isize = Simulation::getInstance()->infrastructure.intersection_size;
    boundingBox.position = {position.x - isize / 2.f, position.y - isize / 2.f};
    boundingBox.size = {isize, isize};

    if (!has_traffic_lights)
        return;

    float offset = isize / 2.f;
    float size   = isize / 3.f;
    float thickness = 5.f;

    // setup light boxes
    if (active_light_directions[0])
    {
        light_boxes[0].position = {position.x - offset, position.y - offset - thickness};
        light_boxes[0].size = {size, thickness};
    }
    if (active_light_directions[1])
    {
        light_boxes[1].position = {position.x + offset + thickness, position.y - offset};
        light_boxes[1].size = {-thickness, size};
    }
    if (active_light_directions[2])
    {
        light_boxes[2].position = {position.x + offset, position.y + offset + thickness};
        light_boxes[2].size = {-size, -thickness};
    }
    if (active_light_directions[3])
    {
        light_boxes[3].position = {position.x - offset - thickness, position.y + offset};
        light_boxes[3].size = {thickness, -size};
    }
}

void Intersection::SpawnLightBoxes()
{
    // setup active lights based on infrastructure map
    int map_size = Simulation::getInstance()->infrastructure.map_size;
    int intersection_count = Simulation::getInstance()->infrastructure.intersection_count;
    auto &infra_map = Simulation::getInstance()->infrastructure.infrastructure_map;

    int x = id % map_size;

    active_light_directions[0] = (id >= map_size) && (infra_map[id - map_size][id] == 1);
    active_light_directions[1] = (x < map_size - 1) && (infra_map[id + 1][id] == 1);
    active_light_directions[2] = (id + map_size < intersection_count) && (infra_map[id + map_size][id] == 1);
    active_light_directions[3] = (x > 0) && (infra_map[id - 1][id] == 1);

    // disable lights check — count all connected roads (both directions)
    int total_roads = 0;
    if (id >= map_size           && (infra_map[id - map_size][id] == 1 || infra_map[id][id - map_size] == 1)) total_roads++;
    if (x < map_size - 1         && (infra_map[id + 1][id] == 1        || infra_map[id][id + 1]        == 1)) total_roads++;
    if (id + map_size < intersection_count && (infra_map[id + map_size][id] == 1 || infra_map[id][id + map_size] == 1)) total_roads++;
    if (x > 0                    && (infra_map[id - 1][id] == 1        || infra_map[id][id - 1]        == 1)) total_roads++;

    if (total_roads <= 2)
    {
        has_traffic_lights = false;
        return;
    }

    // this shouldnt happen
    if (!active_light_directions[0] &&
        !active_light_directions[1] &&
        !active_light_directions[2] &&
        !active_light_directions[3])
    {
        has_traffic_lights = false;
        return;
    }
}

void Intersection::Update()
{
    if (!has_traffic_lights)
        return;

    if (_time >= 120)
    {
        do
        {
            current_green_light_direction++;
            if (current_green_light_direction > 3)
                current_green_light_direction = 0;
        } while (!active_light_directions[current_green_light_direction]);

        _time = 0;
    }
    _time++;   
}

void Intersection::Draw()
{
    // draw intersections
    sf::RectangleShape intersection_shape({Simulation::getInstance()->infrastructure.intersection_size, Simulation::getInstance()->infrastructure.intersection_size});
    intersection_shape.setPosition({position.x - Simulation::getInstance()->infrastructure.intersection_size / 2, position.y - Simulation::getInstance()->infrastructure.intersection_size / 2});

    if (used)
    {
        intersection_shape.setFillColor(sf::Color(90, 90, 90));
    }
    else
    {
        intersection_shape.setFillColor(sf::Color(20, 20, 20));
    }

    GuiManager::getInstance()->window.draw(intersection_shape);

    GuiManager::getInstance()->DrawDebugText(to_string(id), position);

    if ((bool)GuiManager::getInstance()->draw_debug)
    {
        sf::RectangleShape boundingRect;
        boundingRect.setPosition(boundingBox.position);
        boundingRect.setSize(boundingBox.size);
        boundingRect.setFillColor(sf::Color::Transparent);
        boundingRect.setOutlineColor(sf::Color::Cyan);
        boundingRect.setOutlineThickness(1.0f);
        GuiManager::getInstance()->window.draw(boundingRect);
    }

    if (!has_traffic_lights)
        return;

    for (int i = 0; i < 4; i++)
    {
        if (active_light_directions[i])
        {
            // draw light circles
            sf::CircleShape lightShape;
            lightShape.setPosition(light_boxes[i].position);
            lightShape.setRadius(4.f);
            lightShape.setPointCount(8);
            if (current_green_light_direction == i)
                lightShape.setFillColor(sf::Color::Green);
            else
                lightShape.setFillColor(sf::Color::Red);
            GuiManager::getInstance()->window.draw(lightShape);

            if ((bool)GuiManager::getInstance()->draw_debug)
            {
                sf::RectangleShape boundingRect;
                boundingRect.setPosition(light_boxes[i].position);
                boundingRect.setSize(light_boxes[i].size);
                boundingRect.setFillColor(sf::Color::Transparent);
                boundingRect.setOutlineColor(sf::Color::White);
                boundingRect.setOutlineThickness(1.0f);
                GuiManager::getInstance()->window.draw(boundingRect);
            }
        }
    }
}
