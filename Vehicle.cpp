#include <SFML/Graphics.hpp>
#include "Vehicle.h"
#include "Simulation.h"
#include "Intersection.h"
#include "GuiManager.h"

using namespace std;

Vehicle::Vehicle(float _max_speed, float _car_length, float _accerleration, float _breaking_force, int _start_intersection_id, int _finish_intersection_id){
    max_speed = _max_speed;
    car_length = _car_length;
    accerleration = _accerleration;
    breaking_force = _breaking_force;
    start_intersection_id = _start_intersection_id;
    finish_intersection_id = _finish_intersection_id;
}

void Vehicle::Setup(){
    speed = 0;
    path = CalculatePath();
    AdvanceToNextIntersection();

    Intersection& current = Simulation::getInstance()->infrastructure.intersections[current_intersection_id];
    Intersection& next = Simulation::getInstance()->infrastructure.intersections[next_intersection_id];

    sf::Vector2f dir = next.position - current.position;
    dir = dir / dir.length();
    sf::Vector2f right = {-dir.y, dir.x};

    position = current.position + right * 17.5f;
}

// TODO: replace with proper pathfinding algo
vector<int> Vehicle::CalculatePath(){
    vector<int> result_path;
    result_path.push_back(3);
    result_path.push_back(0);
    result_path.push_back(1);
    result_path.push_back(4);
    result_path.push_back(7);
    result_path.push_back(8);
    result_path.push_back(5);
    return result_path;
}

void Vehicle::AdvanceToNextIntersection(){
    current_intersection_id = path[0];
    next_intersection_id = path[1];
    turning_direction = CalculateTurningDirection();

    if(path.size() > 1){
        path.erase(path.begin());
    }else{
        // finished path, delete vechicle
    }
}

int Vehicle::CalculateTurningDirection(){
    // next intersection is the finish line, not turning
    if(path.size() < 3){
        return -1;
    }

    Intersection& current = Simulation::getInstance()->infrastructure.intersections[current_intersection_id];
    Intersection& next = Simulation::getInstance()->infrastructure.intersections[next_intersection_id];
    Intersection& after_next = Simulation::getInstance()->infrastructure.intersections[path[2]];

    sf::Vector2f v1 = next.position - current.position;
    sf::Vector2f v2 = after_next.position - next.position;

    int cross = v1.x * v2.y - v1.y * v2.x;

    if (cross > 0) return 2;
    if (cross < 0) return 0;
    if (cross == 0) return 1;

    return -1;
}

void Vehicle::Update(){
    Intersection& next_intersection = Simulation::getInstance()->infrastructure.intersections[next_intersection_id];
    Intersection& current_intersection = Simulation::getInstance()->infrastructure.intersections[current_intersection_id];

    if (speed < max_speed) speed += accerleration;
    if (speed > max_speed) speed = max_speed;

    bool bezier_active = false;

    if((bool)boundingBox.findIntersection(next_intersection.boundingBox)){
        if(path.size() <= 1){
            return;
        }

        if(!is_turning){
            is_turning = true;
            entry_angle = moving_angle;
            entry_position = position;
            turn_t = 0.f;

            if (path.size() >= 2) {
                sf::Vector2f after_next_pos = Simulation::getInstance()->infrastructure.intersections[path[1]].position;
                exit_angle = (after_next_pos - next_intersection.position).angle();

                sf::Vector2f entry_dir = {std::cos(entry_angle.asRadians()), std::sin(entry_angle.asRadians())};
                sf::Vector2f exit_dir = {std::cos(exit_angle.asRadians()), std::sin(exit_angle.asRadians())};
                sf::Vector2f exit_right = {-exit_dir.y, exit_dir.x};

                exit_point = next_intersection.position + exit_right * 17.5f + exit_dir * (next_intersection.intersection_size / 2.f);

                float det = entry_dir.x * exit_dir.y - exit_dir.x * entry_dir.y;
                sf::Vector2f delta = exit_point - entry_position;

                if (std::abs(det) < 0.01f) {
                    control_point = (entry_position + exit_point) / 2.f;
                } else {
                    float tp = (delta.x * exit_dir.y - delta.y * exit_dir.x) / det;
                    control_point = entry_position + entry_dir * tp;
                }

                turn_arc_length = (control_point - entry_position).length() + (exit_point - control_point).length();
                if (turn_arc_length < 0.001f) turn_arc_length = 1.f;
            }
        }

        turn_t += speed / turn_arc_length;

        if (turn_t < 1.f) {
            float t = turn_t;
            position = (1.f-t)*(1.f-t)*entry_position + 2.f*t*(1.f-t)*control_point + t*t*exit_point;
            sf::Vector2f tangent = (1.f-t)*(control_point-entry_position) + t*(exit_point-control_point);
            if (tangent.length() > 0.001f) moving_angle = tangent.angle();
            bezier_active = true;
        } else {
            sf::Vector2f tangent = exit_point - control_point;
            if (tangent.length() > 0.001f) moving_angle = tangent.angle();
        }

    }else{
        if (is_turning) {
            AdvanceToNextIntersection();

            Intersection& new_current = Simulation::getInstance()->infrastructure.intersections[current_intersection_id];
            Intersection& new_next = Simulation::getInstance()->infrastructure.intersections[next_intersection_id];

            sf::Vector2f road_dir = new_next.position - new_current.position;
            road_dir = road_dir / road_dir.length();
            sf::Vector2f right = {-road_dir.y, road_dir.x};

            sf::Vector2f to_pos = position - new_current.position;
            float along = to_pos.x * road_dir.x + to_pos.y * road_dir.y;
            position = new_current.position + road_dir * along + right * 17.5f;

            moving_angle = exit_angle;
            is_turning = false;
        }else{
            if (path.size() >= 2) {
                sf::Vector2f moving_direction = next_intersection.position - current_intersection.position;
                moving_angle = moving_direction.angle();
            }
        }
    }

    debug_text = to_string(path.size());

    if (!bezier_active) {
        sf::Vector2f velocity = sf::Vector2f(std::cos(moving_angle.asRadians()), std::sin(moving_angle.asRadians())) * speed;
        position += velocity;
    }
}

void Vehicle::Draw(){
    // draw the vehicle
    sf::RectangleShape vehicle_shape({car_length, car_width});
    vehicle_shape.setOrigin({car_length / 2.f, car_width / 2.f});
    vehicle_shape.setPosition(position);
    vehicle_shape.setRotation(moving_angle);
    GuiManager::getInstance()->window.draw(vehicle_shape);

    // draw bounding box
    boundingBox = vehicle_shape.getGlobalBounds();
    sf::RectangleShape boundingRect;
    boundingRect.setPosition(boundingBox.position);
    boundingRect.setSize(boundingBox.size);
    boundingRect.setFillColor(sf::Color::Transparent);
    boundingRect.setOutlineColor(sf::Color::Cyan);
    boundingRect.setOutlineThickness(1.f);
    GuiManager::getInstance()->window.draw(boundingRect);

    GuiManager::getInstance()->DrawText(debug_text, position);
}
