#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Simulation;

class Intersection {
    public:

    int id;
    float intersection_size = 70.f;
    sf::Vector2f position; // center point of intersection
    bool has_traffic_lights;
    bool used;
    sf::FloatRect boundingBox;

    Intersection(int _id, sf::Vector2f _position, bool _has_traffic_lights);

    void Update();

    void Draw();
};
