#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class Simulation;

class Road {
    public:

    int intersection_a_id;
    int intersection_b_id;
    float speed_limit, road_length;
    sf::FloatRect boundingBox;
    sf::Vector2f line_start_position;
    sf::Angle intersections_angle;

    Road(int _intersection_a_id, int _intersection_b_id, float _speed_limit);

    void Setup();

    void UpdateGlobalPosition(); // recompute line_start_position from intersection global positions

    void Update();

    void Draw();
};
