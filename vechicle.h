#include <SFML/Graphics.hpp>

using namespace std;

class Vechicle{
    private:

    // list of intersection ids
    vector<int> path;
    int current_intersection_id;
    int next_intersection_id;

    public:

    float length;
    float accerleration;
    float max_speed;
    float breaking_force;

    int start_intersection_id;
    int finish_intersection_id;

    float speed;
    sf::Vector2f position = sf::Vector2f(10.f, 10.f);
    sf::Angle moving_angle;

    Vechicle(float _max_speed, float _length, float _accerleration, float _breaking_force, int _start_intersection_id, int _finish_intersection_id){
        max_speed = _max_speed;
        length = _length;
        accerleration = _accerleration;
        breaking_force = _breaking_force;
        start_intersection_id = _start_intersection_id;
        finish_intersection_id = _finish_intersection_id;
    }

    void setup(Infrastructure& infrastructure){
        speed = 0;

        path = calculate_path();
        advance_next_intersection();
        position = infrastructure.intersections[current_intersection_id].position;
    }

    // TODO: replace with proper pathfinding algo
    vector<int> calculate_path(){
        vector<int> result_path;
        result_path.push_back(3);
        result_path.push_back(0);
        result_path.push_back(1);
        result_path.push_back(4);
        result_path.push_back(3);
        result_path.push_back(4);
        return result_path;
    }

    // function to set
    void advance_next_intersection(){
        current_intersection_id = path[0];
        next_intersection_id = path[1];
        if(path.size() > 2){
            path.erase(path.begin());
        }else{
            // finished path, delete vechicle
        }
    }

    void update(Infrastructure& infrastructure){
        // update speed
        if (speed < max_speed) speed += accerleration;
        if (speed > max_speed) speed = max_speed;

        // here function to get next target from the list

        sf::Vector2f target_position = infrastructure.intersections[next_intersection_id].position;
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
            advance_next_intersection();
        }
    }
};
