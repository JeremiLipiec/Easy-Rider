#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class Vehicle{
private:

    // list of intersection ids
    vector<int> path;
    int current_intersection_id;
    int next_intersection_id;
    float car_width = 20.f;

    // oh god
    float right_turn_radius = 10.f;
    float left_turn_radius = 20.f;

    sf::Angle entry_angle;
    sf::Angle exit_angle;
    sf::Vector2f entry_position;
    bool is_turning = false;

public:

    float car_length;
    float accerleration;
    float max_speed;
    float breaking_force;

    int start_intersection_id;
    int finish_intersection_id;
    int turning_direction = -1; // 0 LEFT, 1 STRAIGHT, 2 RIGHT, -1 END OF PATH
    string debug_text = "";


    float speed;
    sf::Vector2f position = sf::Vector2f(10.f, 10.f);
    sf::Angle moving_angle;
    sf::FloatRect boundingBox;

    Vehicle(float _max_speed, float _length, float _accerleration, float _breaking_force, int _start_intersection_id, int _finish_intersection_id);

    void Setup();

    void Update();

    void Draw();

    int CalculateTurningDirection();

    vector<int> CalculatePath();

    void AdvanceToNextIntersection();
};
