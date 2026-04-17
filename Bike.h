#pragma once
#include "Vehicle.h"

class Bike : public Vehicle {
public:
    Bike(int start_id, int finish_id) : Vehicle(2.f, 17.5f, 10.f, 0.1f, 0.15f, start_id, finish_id) {}
};
