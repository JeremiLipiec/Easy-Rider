#pragma once

#include <SFML/Graphics.hpp>
#include "Infrastructure.h"
#include "Traffic.h"

using namespace std;

class Simulation{
private:

    static Simulation* FInstance;

    Simulation(int _intersection_count);

public:

    Infrastructure infrastructure;
    Traffic traffic;

    static Simulation* getInstance(int _intersection_count = 0);

    void Draw(sf::RenderWindow& window);

    void Update();

    void AddVehicle(float max_speed, float length, float accerleration, float breaking_force, int start_intersection_id, int finish_intersection_id);
};
