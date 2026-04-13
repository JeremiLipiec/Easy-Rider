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
    position = Simulation::getInstance()->infrastructure.intersections[current_intersection_id].position;
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

    // update speed
    if (speed < max_speed) speed += accerleration;
    if (speed > max_speed) speed = max_speed;

    // is on the next intersection
    if((bool)boundingBox.findIntersection(next_intersection.boundingBox)){
        // arrived at the finish intersection
        if(path.size() <= 1){
            return;
        }

        // just entered the intersection, save initial position and angle
        if(!is_turning){
            is_turning = true;
            entry_angle = moving_angle;
            entry_position = position;

            // calculate exit angle and position
            if (path.size() >= 2) {
                sf::Vector2f after_next_intersection_position = Simulation::getInstance()->infrastructure.intersections[path[1]].position;
                exit_angle = (after_next_intersection_position - next_intersection.position).angle();
            }
        }

        float dist_from_entry = (position - entry_position).length();
        float t = dist_from_entry / (next_intersection.intersection_size);

        // clamp value
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;

        // values to interpolate
        float start_angle_deg = entry_angle.asDegrees();
        float end_angle_deg = exit_angle.asDegrees();

        if (end_angle_deg - start_angle_deg > 180) start_angle_deg += 360;
        if (end_angle_deg - start_angle_deg < -180) start_angle_deg -= 360;

        moving_angle = sf::degrees(start_angle_deg + (end_angle_deg - start_angle_deg) * t);

    }else{
        // just left the intersection
        if (is_turning) {
            AdvanceToNextIntersection();
            sf::Vector2f target = next_intersection.position;
            //float rad = exit_angle.asRadians();
            //sf::Vector2f forward_vec(std::cos(rad), std::sin(rad));

            //position = target + forward_vec * (half_size + car_length / 2.0f);

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

    // apply velocity
    sf::Vector2f velocity = sf::Vector2f(std::cos(moving_angle.asRadians()), std::sin(moving_angle.asRadians())) * speed;
    position += velocity;
    position +=
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
