#pragma once

#include <SFML/Graphics.hpp>
#include "Infrastructure.h"

using namespace std;

class Vehicle
{
private:
    // list of intersection ids
    vector<int> path;
    int current_intersection_id;
    int next_intersection_id;
    float car_width = 20.f;

    sf::Angle entry_angle;
    sf::Angle exit_angle;
    sf::Vector2f entry_position;
    sf::Vector2f exit_point;
    sf::Vector2f control_point;
    float turn_t = 0.f;
    float turn_arc_length = 1.f;
    float turn_speed_limit = 0.f;
    bool is_turning = false;

    // car collision check and "vision"
    sf::Vector2f collision_point_front_position = {0, 0};
    sf::Vector2f collision_point_mask_position = {0, 0};

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
    int spawn_timer = 0;
    bool is_spawned = false;
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

    bool PointsCollidingWithCar();

    bool PointsCollidingWithRedLight(Intersection next_intersection);
};
