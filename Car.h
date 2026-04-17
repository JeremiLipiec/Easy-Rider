#pragma once
#include "Vehicle.h"

class Car : public Vehicle {
public:
    Car(int start_id, int finish_id) : Vehicle(2.f, 35.f, 20.f, 0.05f, 0.1f, start_id, finish_id) {}
};
