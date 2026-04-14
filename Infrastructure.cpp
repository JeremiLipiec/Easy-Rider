#include <SFML/Graphics.hpp>
#include <cmath>
#include "Infrastructure.h"
#include "GuiManager.h"

using namespace std;

Infrastructure::Infrastructure(){}

void Infrastructure::Setup(int _intersection_count){
    intersection_count = _intersection_count;
    map_size = sqrt(intersection_count);
    infrastructure_map = vector<vector<int>>(intersection_count, vector<int>(intersection_count, 0));
}

void Infrastructure::FillMap(){
    int col = 0, row = 0, x = 0, a = 0;

    for(row = 0; row < map_size; row++){
        a = map_size * row;
        for(x = 0; x < map_size - 1; x++){
            infrastructure_map[a + x][a + x + 1] = 1;
            infrastructure_map[a + x + 1][a + x] = 1;
        }
    }

    for(col = 0; col < map_size; col++){
        for(x = 0; x < map_size - 1; x++){
            a = col + x * map_size;
            infrastructure_map[a][a + map_size] = 1;
            infrastructure_map[a + map_size][a] = 1;
        }
    }
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
                if(infrastructure_map[x][y] == 1 && x != y){
                    roads.push_back(Road(intersections[x].id, intersections[y].id, 50));
                    intersections[x].used = true;
                    intersections[y].used = true;
                }
            }
        }
    }

    void Infrastructure::Update(){
        for(Intersection& i : intersections){
            i.Update();
        }
    }

    void Infrastructure::Draw(){
        for(Road& r : roads){
            r.Draw();
        }

        for(Intersection& i : intersections){
            i.Draw();
        }
    }



