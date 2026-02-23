#include <SFML/Graphics.hpp>
#include <cmath>
#include "Infrastructure.h"
#include "GuiManager.h"

using namespace std;

    Infrastructure::Infrastructure(){}

    Infrastructure::Infrastructure(int _intersection_count){
        Setup(_intersection_count);
    }

    void Infrastructure::Setup(int _intersection_count){
        intersection_count = _intersection_count;
        map_size = sqrt(intersection_count);
        infrastructure_map = vector<vector<int>>(intersection_count, vector<int>(intersection_count, 0));
    }

    void Infrastructure::GenerateMap(){
        int intersection_id = 0;

        for(int y = 0; y < map_size; y ++){
            for(int x = 0; x < map_size; x ++){
                sf::Vector2f intersection_position(drawing_start_x + intersection_distance * (intersection_id % map_size), drawing_start_y + intersection_distance * (intersection_id / map_size));
                intersections.push_back(Intersection(intersection_id++, intersection_position, true));
            }
        }

        for(int y = 0; y < intersection_count; y ++){
            for(int x = 0; x < intersection_count; x ++){
                if(infrastructure_map[x][y] == 1){
                    roads.push_back(Road(intersections[x].id, intersections[y].id, 50));
                    intersections[x].used = true;
                    intersections[y].used = true;
                }
            }
        }
    }

    void Infrastructure::Update(){
        // to be implemented: update traffic signals change ect
    }

    void Infrastructure::Draw(sf::RenderWindow& window){
        sf::RectangleShape intersection_shape({intersection_size, intersection_size});

        // draw roads
        for(Road& r : roads){
            // calculate distance between intersections
            sf::Vector2f intersection_diff = intersections[r.intersection_b_id].position - intersections[r.intersection_a_id].position;
            float road_length = intersection_diff.length();

            // calculate angle between intersections
            sf::Angle intersections_angle = intersection_diff.angle();

            // calculate starting position
            float angle_shift_x = (sin(intersections_angle.asDegrees()*3.1415/180)/2 + 1/2.f) * road_thickness - road_thickness / 2; // ???
            float angle_shift_y = (cos(intersections_angle.asDegrees()*3.1415/180)/2 + 1/2.f) * road_thickness - road_thickness / 2; // ???

            float angle_start_position_shift_x = angle_shift_x;
            float angle_start_position_shift_y = angle_shift_y;

            float line_start_pos_x = intersections[r.intersection_a_id].position.x + intersection_size / 2 + angle_start_position_shift_x;
            float line_start_pos_y = intersections[r.intersection_a_id].position.y + intersection_size / 2 - angle_start_position_shift_y;

            sf::Vector2f line_start_position(line_start_pos_x, line_start_pos_y);

            // draw the rectangle
            sf::RectangleShape road_shape({road_length, road_thickness});
            road_shape.setFillColor(sf::Color(100, 100, 100));
            road_shape.setPosition(line_start_position);
            road_shape.setRotation(intersections_angle);
            window.draw(road_shape);
        }

        // draw intersections
        for(int i = 0; i < intersection_count; i++){
            intersection_shape.setPosition(intersections[i].position);

            if(intersections[i].used){
                intersection_shape.setFillColor(sf::Color(90, 90, 90));
            }else{
                intersection_shape.setFillColor(sf::Color(20, 20, 20));
            }

            window.draw(intersection_shape);

            GuiManager::getInstance()->DrawText(to_string(i), intersections[i].position, window);

            //sf::FloatRect boundingBox = intersection_shape.getGlobalBounds();
            //sf::RectangleShape debugRect;
            //debugRect.setPosition(boundingBox.position);
            //debugRect.setSize(boundingBox.size);
            //debugRect.setFillColor(sf::Color::Transparent);
            //debugRect.setOutlineColor(sf::Color::Cyan);
            //debugRect.setOutlineThickness(1.0f);
            //window.draw(debugRect);
        }
    }



