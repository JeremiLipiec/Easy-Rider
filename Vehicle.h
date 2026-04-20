#pragma once

#include <SFML/Graphics.hpp>
#include "Infrastructure.h"
#include "TrafficLight.h"

using namespace std;

class Vehicle
{
private:
    // list of intersection ids
    vector<int> path;
    int current_intersection_id = 0;
    int next_intersection_id = 0;

protected:
    float car_width = 20.f;
    string texture_path;
    sf::Texture texture;

private:
    // turning vars
    sf::Vector2f entry_point;
    sf::Vector2f exit_point;
    float turning_radius = 0.f;
    float turn_t = 0.f;
    bool is_turning = false;
    sf::Angle entry_angle;

    // car collision check and "vision"
    sf::Vector2f collision_point_front_position = {0, 0};
    sf::Vector2f collision_point_mask_position = {0, 0};
    // dont go onto intersection if cannot leave it
    sf::Vector2f collision_point_left_turn_position = {0, 0};
    sf::Vector2f collision_point_right_turn_position = {0, 0};
    sf::Vector2f collision_point_straight_position = {0, 0};

public:
    float car_length;
    float accerleration;
    float max_speed;
    float breaking_force;

    int start_intersection_id;
    int finish_intersection_id;
    int turning_direction = -1; // 0 LEFT, 1 STRAIGHT, 2 RIGHT, 3 U-TURN, -1 END OF PATH
    string debug_text = "";

    float speed;
    int spawn_timer = 0;
    bool is_spawned = false;
    bool deleted = false;
    sf::Vector2f position = sf::Vector2f(10.f, 10.f);
    sf::Vector2f global_position = sf::Vector2f(10.f, 10.f);
    sf::Angle moving_angle;
    sf::FloatRect boundingBox;

    sf::Vector2f forward = {1.f,  0.f};
    sf::Vector2f right = {0.f,  1.f};
    sf::Vector2f left = {0.f, -1.f};

    Vehicle(float _max_speed, float _length, float _car_width, float _accerleration, float _breaking_force, int _start_intersection_id, int _finish_intersection_id);

    void Setup();

    void Update();

    void Draw();

    int CalculateTurningDirection();

    vector<int> CalculatePath();

    void AdvanceToNextIntersection();

    bool PointsCollidingWithCar();

    TrafficLight PointsCollidingWithLight(Intersection on_intersection);

    void CheckRemoveClick();
};
