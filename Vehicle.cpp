#include <SFML/Graphics.hpp>
#include "Vehicle.h"
#include "Simulation.h"

using namespace std;

Vehicle::Vehicle(float _max_speed, float _length, float _accerleration, float _breaking_force, int _start_intersection_id, int _finish_intersection_id){
    max_speed = _max_speed;
    length = _length;
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
    result_path.push_back(3);
    result_path.push_back(4);
    return result_path;
}

void Vehicle::AdvanceToNextIntersection(){
    current_intersection_id = path[0];
    next_intersection_id = path[1];
    if(path.size() > 2){
        path.erase(path.begin());
    }else{
        // finished path, delete vechicle
    }
}

void Vehicle::Update(){
    // update speed
    if (speed < max_speed) speed += accerleration;
    if (speed > max_speed) speed = max_speed;

    // here function to get next target from the list

    sf::Vector2f target_position = Simulation::getInstance()->infrastructure.intersections[next_intersection_id].position;
    sf::Vector2f moving_direction = target_position - position;
    float distance = moving_direction.length();

    if(distance > 1.f){
        moving_angle = moving_direction.angle();
    }

    // cast a ray to the point of full stop from current speed, and if another car is there start slowing down untill the car is no more in the cast
    // how the f do i program traffic lights and intersections???????

    if (distance > 1.f) {
        sf::Vector2f velocity = (moving_direction / distance) * speed; // normalize
        position += velocity;
    } else {
        // arrived ad the next intersection
        speed = 0;
        position = target_position;
        AdvanceToNextIntersection();
    }
}
