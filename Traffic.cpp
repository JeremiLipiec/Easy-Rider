#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "Traffic.h"

using namespace std;

    void Traffic::AddVehicle(float max_speed, float length, float accerleration, float breaking_force, int start_intersection_id, int finish_intersection_id){
        vehicles.push_back(Vehicle(max_speed, length, accerleration, breaking_force, start_intersection_id, finish_intersection_id));
        vehicles.back().Setup();
    }

    void Traffic::Update(){
        for(Vehicle& v : vehicles){
            v.Update();
        }
    }

    void Traffic::Draw(sf::RenderWindow& window){
        for(Vehicle& v : vehicles){
            sf::RectangleShape vehicle_shape({v.length, car_width});

            sf::Vector2 car_position = v.position;
            int car_angle = v.moving_angle.asDegrees() + 90;


            if(car_angle >= 0.f){
                car_position.x += car_width / 2 + Simulation::getInstance()->infrastructure.road_thickness / 2;
            }

            if(car_angle >= 90){
                car_position.y += car_width / 2 + Simulation::getInstance()->infrastructure.road_thickness / 2;
            }

            if(car_angle >= 180){
                car_position.x -= Simulation::getInstance()->infrastructure.road_thickness / 2 - car_width / 2 - 5;
            }

            if(car_angle >= 270){
                car_position.y -= Simulation::getInstance()->infrastructure.road_thickness / 2 - car_width / 2 - 5;
            }

            vehicle_shape.setPosition(car_position);
            vehicle_shape.setRotation(v.moving_angle);
            window.draw(vehicle_shape);


            sf::FloatRect boundingBox = vehicle_shape.getGlobalBounds();
            sf::RectangleShape debugRect;
            debugRect.setPosition(boundingBox.position);
            debugRect.setSize(boundingBox.size);
            debugRect.setFillColor(sf::Color::Transparent);
            debugRect.setOutlineColor(sf::Color::Cyan);
            debugRect.setOutlineThickness(1.0f);
            window.draw(debugRect);
        }
    }

