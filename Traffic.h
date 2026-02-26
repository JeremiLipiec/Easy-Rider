#pragma once

#include <SFML/Graphics.hpp>
#include "Vehicle.h"

using namespace std;

class Traffic{
    public:

    vector<Vehicle> vehicles;

    void AddVehicle(float max_speed, float length, float accerleration, float breaking_force, int start_intersection_id, int finish_intersection_id);

    void Update();

    void Draw();
};
