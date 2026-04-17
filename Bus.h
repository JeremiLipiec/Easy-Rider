#pragma once
#include "Vehicle.h"

class Bus : public Vehicle {
public:
    Bus(int start_id, int finish_id) : Vehicle(1.2f, 52.5f, 20.f, 0.025f, 0.08f, start_id, finish_id) {}
};
