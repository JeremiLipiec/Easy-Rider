#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class Simulation;

class Road {
    public:

    int intersection_a_id;
    int intersection_b_id;
    float speed_limit;
    float road_thickness = 70.f;

    Road(int _intersection_a_id, int _intersection_b_id, float _speed_limit);

    void Update();

    void Draw();
};
