#include <SFML/Graphics.hpp>
#include "infrastructure.h"
#include "vechicle.h"

using namespace std;

class Simulation{
    public:

    Infrastructure infrastructure;
    vector<Vechicle> vechicles;
    float car_width = 20.f;

    Simulation(int _intersection_count){
        infrastructure.setup(_intersection_count);
    }

    void draw(sf::RenderWindow& window){
        infrastructure.draw_map(window);
        draw_vechicles(window);
    }

    void add_vechicle(float max_speed, float length, float accerleration, float breaking_force, int start_intersection_id, int finish_intersection_id){
        vechicles.push_back(Vechicle(max_speed, length, accerleration, breaking_force, start_intersection_id, finish_intersection_id));
        vechicles.back().setup(infrastructure);
    }

    void update(){
        for(Vechicle& v : vechicles){
            v.update(infrastructure);
        }
    }

    void draw_vechicles(sf::RenderWindow& window){
        for(Vechicle& v : vechicles){
            sf::RectangleShape vechicle_shape({v.length, car_width});

            sf::Vector2 car_position = v.position;
            int car_angle = v.moving_angle.asDegrees() + 90;


            if(car_angle >= 0.f){
                car_position.x += car_width / 2 + infrastructure.road_thickness / 2;
            }

            if(car_angle >= 90){
                car_position.y += car_width / 2 + infrastructure.road_thickness / 2;
            }

            if(car_angle >= 180){
                car_position.x -= infrastructure.road_thickness / 2 - car_width / 2 - 5;
            }

            if(car_angle >= 270){
                car_position.y -= infrastructure.road_thickness / 2 - car_width / 2 - 5;
            }

            vechicle_shape.setPosition(car_position);
            vechicle_shape.setRotation(v.moving_angle);
            window.draw(vechicle_shape);


            sf::FloatRect boundingBox = vechicle_shape.getGlobalBounds();
            sf::RectangleShape debugRect;
            debugRect.setPosition(boundingBox.position);
            debugRect.setSize(boundingBox.size);
            debugRect.setFillColor(sf::Color::Transparent);
            debugRect.setOutlineColor(sf::Color::Cyan);
            debugRect.setOutlineThickness(1.0f);
            window.draw(debugRect);
        }
    }
};
