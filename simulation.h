#include <SFML/Graphics.hpp>
#include "infrastructure.h"
#include "vechicle.h"

using namespace std;

class Simulation{
    public:

    Infrastructure infrastructure;
    vector<Vechicle> vechicles;

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
            sf::RectangleShape vechicle_shape({v.length, 20.f});
            vechicle_shape.setPosition(v.position);
            window.draw(vechicle_shape);
        }
    }
};
