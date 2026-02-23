#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class Vehicle{
private:

    // list of intersection ids
    vector<int> path;
    int current_intersection_id;
    int next_intersection_id;

public:

    float length;
    float accerleration;
    float max_speed;
    float breaking_force;

    int start_intersection_id;
    int finish_intersection_id;

    float speed;
    sf::Vector2f position = sf::Vector2f(10.f, 10.f);
    sf::Angle moving_angle;

    Vehicle(float _max_speed, float _length, float _accerleration, float _breaking_force, int _start_intersection_id, int _finish_intersection_id);

    void Setup();

    void Update();

    vector<int> CalculatePath();

    void AdvanceToNextIntersection();
};
