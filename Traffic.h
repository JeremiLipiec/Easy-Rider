#pragma once

#include <SFML/Graphics.hpp>
#include "Vehicle.h"
#include "Car.h"
#include "Bike.h"
#include "Bus.h"

using namespace std;

class Traffic{
    public:

    vector<Vehicle> vehicles;

    void AddCar(int start_id, int finish_id);
    void AddBike(int start_id, int finish_id);
    void AddBus(int start_id, int finish_id);

    void Update();

    void Draw();
};
