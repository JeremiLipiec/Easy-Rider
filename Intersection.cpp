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

    for (int i = 0; i < 4; i++)
        traffic_lights[i].UpdatePosition(position, isize, i);
}

void Intersection::SpawnLightBoxes()
{
    int map_size = Simulation::getInstance()->infrastructure.map_size;
    int intersection_count = Simulation::getInstance()->infrastructure.intersection_count;
    vector<vector<int>> &infra_map = Simulation::getInstance()->infrastructure.infrastructure_map;

    int x = id % map_size;

    traffic_lights[0].active = (id >= map_size) && (infra_map[id - map_size][id] == 1);
    traffic_lights[1].active = (x < map_size - 1) && (infra_map[id + 1][id] == 1);
    traffic_lights[2].active = (id + map_size < intersection_count) && (infra_map[id + map_size][id] == 1);
    traffic_lights[3].active = (x > 0) && (infra_map[id - 1][id] == 1);

    int total_roads = 0;
    if (id >= map_size && (infra_map[id - map_size][id] == 1 || infra_map[id][id - map_size] == 1))
        total_roads++;
    if (x < map_size - 1 && (infra_map[id + 1][id] == 1 || infra_map[id][id + 1] == 1))
        total_roads++;
    if (id + map_size < intersection_count && (infra_map[id + map_size][id] == 1 || infra_map[id][id + map_size] == 1))
        total_roads++;
    if (x > 0 && (infra_map[id - 1][id] == 1 || infra_map[id][id - 1] == 1))
        total_roads++;

    if (total_roads <= 2)
    {
        has_traffic_lights = false;
        for (int i = 0; i < 4; i++) traffic_lights[i].active = false;
        return;
    }

    if (!traffic_lights[0].active && !traffic_lights[1].active &&
        !traffic_lights[2].active && !traffic_lights[3].active)
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
            if (current_green_light_direction > 7)
                current_green_light_direction = 0;
        } while (!traffic_lights[current_green_light_direction / 2].active);

        _time = 0;
    }
    _time++;

    for (int i = 0; i < 4; i++)
    {
        if (!traffic_lights[i].active)
            continue;
        if (current_green_light_direction / 2 == i && current_green_light_direction % 2 == 1)
            traffic_lights[i].color = TrafficLight::Color::Green;
        else if (current_green_light_direction / 2 == i && current_green_light_direction % 2 == 0)
            traffic_lights[i].color = TrafficLight::Color::Yellow;
        else
            traffic_lights[i].color = TrafficLight::Color::Red;
    }
}

void Intersection::Draw()
{
    sf::RectangleShape intersection_shape({Simulation::getInstance()->infrastructure.intersection_size, Simulation::getInstance()->infrastructure.intersection_size});
    intersection_shape.setPosition({position.x - Simulation::getInstance()->infrastructure.intersection_size / 2, position.y - Simulation::getInstance()->infrastructure.intersection_size / 2});

    if (used)
        intersection_shape.setFillColor(sf::Color(90, 90, 90));
    else
        intersection_shape.setFillColor(sf::Color(20, 20, 20));

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
        traffic_lights[i].Draw();
}
