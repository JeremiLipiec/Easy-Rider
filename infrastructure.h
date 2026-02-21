#include <SFML/Graphics.hpp>
#include "Road.h"
#include "Intersection.h"
#include <cmath>

using namespace std;

sf::Font font;
sf::Text text(font);

class Infrastructure {
    public:

    float drawing_start_x = 100.f;
    float drawing_start_y = 100.f;
    float intersection_distance = 100.f;

    int intersection_count; // 4
    int map_size;
    float road_thickness = 20.f;
    float intersection_size = 10.f;


    vector<vector<int>> infrastructure_map; // macierz sasiedztwa
    vector<Road> roads;
    vector<Intersection> intersections;

    Infrastructure(int _intersection_count){
        intersection_count = _intersection_count;
        map_size = sqrt(intersection_count);
        infrastructure_map = vector<vector<int>>(intersection_count, vector<int>(intersection_count, 0));
        font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }

    void generate_map(){
        int intersection_id = 0;

        for(int y = 0; y < map_size; y ++){
            for(int x = 0; x < map_size; x ++){
                sf::Vector2f intersection_position(drawing_start_x + intersection_distance * (intersection_id % map_size), drawing_start_y + intersection_distance * (intersection_id / map_size));
                intersections.push_back(Intersection(intersection_id++, intersection_position, true));
            }
        }

        for(int y = 0; y < intersection_count; y ++){
            for(int x = 0; x < intersection_count; x ++){
                if(infrastructure_map[x][y] == 1)
                    roads.push_back(Road(intersections[x].id, intersections[y].id, 50));
            }
        }
    }

    void draw_map(sf::RenderWindow& window){
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

            float line_start_pos_x = intersections[r.intersection_a_id].position.x + intersection_size + angle_start_position_shift_x;
            float line_start_pos_y = intersections[r.intersection_a_id].position.y + intersection_size - angle_start_position_shift_y;

            sf::Vector2f line_start_position(line_start_pos_x, line_start_pos_y);

            sf::RectangleShape line({road_length, road_thickness});
            line.setPosition(line_start_position);
            line.setRotation(intersections_angle);
            line.setFillColor(sf::Color::Green);
            window.draw(line);
        }

        // draw intersections
        for(int y = 0; y < map_size; y ++){
            for(int x = 0; x < map_size; x ++){
                sf::CircleShape shape(intersection_size);
                shape.setPosition(intersections[x + y * map_size].position);
                if(infrastructure_map[x][y] > 0){
                    shape.setFillColor(sf::Color::Blue);
                }else{
                    shape.setFillColor(sf::Color::Red);
                }
                window.draw(shape);

                int px = intersections[x + y * map_size].position.x;
                int py = intersections[x + y * map_size].position.y;

                text.setString(to_string(intersections[x + y * map_size].id));
                text.setPosition({px, py});
                text.setCharacterSize(12);
                text.setFillColor(sf::Color::Cyan);
                window.draw(text);
            }
        }
    }
};



