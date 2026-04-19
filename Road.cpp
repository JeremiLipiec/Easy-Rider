#include <SFML/Graphics.hpp>
#include "Road.h"
#include "GuiManager.h"
#include "Simulation.h"
#include "Intersection.h"
#include <algorithm>

using namespace std;

Road::Road(int _intersection_a_id, int _intersection_b_id, float _speed_limit){
    speed_limit = _speed_limit;
    intersection_a_id = _intersection_a_id;
    intersection_b_id = _intersection_b_id;

    Setup();
}

void Road::Setup(){
    Intersection& intersection_a = Simulation::getInstance()->infrastructure.intersections[intersection_a_id];
    Intersection& intersection_b = Simulation::getInstance()->infrastructure.intersections[intersection_b_id];

    float road_thickness = Simulation::getInstance()->infrastructure.road_thickness;
    float half_intersection = Simulation::getInstance()->infrastructure.intersection_size / 2.f;

    sf::Vector2f diff = intersection_b.position - intersection_a.position;
    float full_length = diff.length();
    intersections_angle = diff.angle();

    sf::Vector2f dir = diff / full_length;
    sf::Vector2f perp_offset = sf::Vector2f(dir.y, -dir.x) * (road_thickness / 2.f);

    road_length = full_length - Simulation::getInstance()->infrastructure.intersection_size;
    line_start_position = intersection_a.position + dir * half_intersection + perp_offset;
}

void Road::UpdateGlobalPosition()
{
    Setup();
}

void Road::Draw(){
    auto &infra = Simulation::getInstance()->infrastructure;
    float thickness = infra.road_thickness;
    bool one_way = (infra.infrastructure_map[intersection_b_id][intersection_a_id] == 0);

    sf::RectangleShape road_shape({road_length, thickness});
    road_shape.setPosition(line_start_position);
    road_shape.setRotation(intersections_angle);
    boundingBox = road_shape.getGlobalBounds();

    // count vehicles on this road
    int count = 0;
    for (auto &v : Simulation::getInstance()->traffic.vehicles)
        if (v.is_spawned && (bool)boundingBox.findIntersection(v.boundingBox))
            count++;

    // calc road tint
    count = count > 2 ? std::clamp(count, 0, 5) : 0;
    float t = count / 5.f;
    sf::Color road_color(static_cast<uint8_t>(165 * (t*t) + 90), 90, 90);

    if (!one_way)
    {
        road_shape.setFillColor(road_color);
        GuiManager::getInstance()->window.draw(road_shape);
    }
    else
    {
        // direction vectors
        sf::Vector2f dir = {std::cos(intersections_angle.asRadians()), std::sin(intersections_angle.asRadians())};
        sf::Vector2f perp_left = {dir.y, -dir.x};

        // excluded lane (left/oncoming half) — dark gray
        sf::RectangleShape excluded({road_length, thickness / 2.f});
        excluded.setPosition(line_start_position);
        excluded.setRotation(intersections_angle);
        excluded.setFillColor(sf::Color(20, 20, 20));
        GuiManager::getInstance()->window.draw(excluded);

        // used lane (right half, direction A→B)
        sf::Vector2f used_start = line_start_position - perp_left * (thickness / 2.f);
        sf::RectangleShape used({road_length, thickness / 2.f});
        used.setPosition(used_start);
        used.setRotation(intersections_angle);
        used.setFillColor(road_color);
        GuiManager::getInstance()->window.draw(used);
    }

    if (GuiManager::getInstance()->draw_debug){
        sf::RectangleShape boundingRect;
        boundingRect.setPosition(boundingBox.position);
        boundingRect.setSize(boundingBox.size);
        boundingRect.setFillColor(sf::Color::Transparent);
        boundingRect.setOutlineColor(sf::Color::Blue);
        boundingRect.setOutlineThickness(1.f);
        GuiManager::getInstance()->window.draw(boundingRect);
    }
}
