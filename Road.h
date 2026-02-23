#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class Simulation;

class Road {
    public:

    int intersection_a_id;
    int intersection_b_id;

    float speed_limit;

    Road(int _intersection_a_id, int _intersection_b_id, float _speed_limit){
        speed_limit = _speed_limit;
        intersection_a_id = _intersection_a_id;
        intersection_b_id = _intersection_b_id;
    }
};
