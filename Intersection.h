#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Simulation;

class Intersection {
    public:

    int id;
    sf::Vector2f local_position; // center point relative to drawing_origin
    sf::Vector2f position; // global screen position = local_position + drawing_origin
    bool has_traffic_lights;
    bool used;
    sf::FloatRect boundingBox;
    int _time;

    // traffic lights, implemented as colliding boxes blocking car travel
    sf::FloatRect light_boxes[4]; // 0Top 1Right 2Down 3Left
    bool active_light_directions[4] = {false, false, false, false}; // 0Top 1Right 2Down 3Left
    short current_green_light_direction; // 0Top yellow, 1Top green, 2Right yellow, 3Right green, 4Down yellow, 5Down green, 6Left yellow, 7Left green

    Intersection(int _id, sf::Vector2f _local_position, bool _has_traffic_lights);

    void UpdateGlobalPosition(); // recompute position, boundingBox, light_boxes from local_position + drawing_origin // call after drawing_origin changes

    void Update();

    void Draw();

    void SpawnLightBoxes();

    void SetupLightDirections(vector<vector<int>>& infra_map, int map_size, int intersection_count);
};
