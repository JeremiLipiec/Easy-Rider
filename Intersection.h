#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Simulation;

class Intersection {
    public:

    int id;
    float intersection_size = 70.f;
    sf::Vector2f position; // center point of intersection
    bool has_traffic_lights;
    bool used;
    sf::FloatRect boundingBox;
    int _time;

    // traffic lights, implemented as colliding boxes blocking car travel
    sf::FloatRect light_boxes[4]; // 0Top 1Right 2Down 3Left
    bool active_light_directions[4] = {false, false, false, false}; // 0Top 1Right 2Down 3Left
    short current_green_light_direction; // 0Top 1Right 2Down 3Left

    Intersection(int _id, sf::Vector2f _position, bool _has_traffic_lights);

    void Update();

    void Draw();

    void SpawnLightBoxes();

    void SetupLightDirections(vector<vector<int>>& infra_map, int map_size, int intersection_count);
};
