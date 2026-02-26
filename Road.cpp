#include <SFML/Graphics.hpp>
#include "Road.h"
#include "GuiManager.h"
#include "Simulation.h"
#include "Intersection.h"

using namespace std;

Road::Road(int _intersection_a_id, int _intersection_b_id, float _speed_limit){
    speed_limit = _speed_limit;
    intersection_a_id = _intersection_a_id;
    intersection_b_id = _intersection_b_id;
}

void Road::Draw(){
    // calculate distance between intersections
    Intersection& intersection_a = Simulation::getInstance()->infrastructure.intersections[intersection_a_id];
    Intersection& intersection_b = Simulation::getInstance()->infrastructure.intersections[intersection_b_id];

    sf::Vector2f intersection_diff = intersection_b.position - intersection_a.position;
    float road_length = intersection_diff.length();

    // calculate angle between intersections
    sf::Angle intersections_angle = intersection_diff.angle();

    // calculate starting position
    float angle_shift_x = (sin(intersections_angle.asDegrees()*3.1415/180)/2 + 1/2.f) * road_thickness - road_thickness / 2; // ???
    float angle_shift_y = (cos(intersections_angle.asDegrees()*3.1415/180)/2 + 1/2.f) * road_thickness - road_thickness / 2; // ???

    float angle_start_position_shift_x = angle_shift_x;
    float angle_start_position_shift_y = angle_shift_y;

    float line_start_pos_x = intersection_a.position.x + angle_start_position_shift_x;
    float line_start_pos_y = intersection_a.position.y - angle_start_position_shift_y;

    sf::Vector2f line_start_position(line_start_pos_x, line_start_pos_y);

    // draw the rectangle
    sf::RectangleShape road_shape({road_length, road_thickness});
    road_shape.setFillColor(sf::Color(100, 100, 100));
    road_shape.setPosition(line_start_position);
    road_shape.setRotation(intersections_angle);
    GuiManager::getInstance()->window.draw(road_shape);
}
