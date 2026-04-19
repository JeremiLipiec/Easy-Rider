#pragma once
#include <SFML/Graphics.hpp>
#include "TrafficLight.h"

using namespace std;

class Simulation;

class Intersection {
public:
    int id;
    sf::Vector2f local_position;
    sf::Vector2f position;
    bool used;
    sf::FloatRect boundingBox;

    bool has_traffic_lights;
    int _time;
    short current_green_light_direction; // value/2=direction, value%2: 0=yellow 1=green
    TrafficLight traffic_lights[4]; // 0Top 1Right 2Down 3Left

    Intersection(int _id, sf::Vector2f _local_position, bool _has_traffic_lights);

    void UpdateGlobalPosition();

    void Update();

    void Draw();

    void SpawnLightBoxes();
};
