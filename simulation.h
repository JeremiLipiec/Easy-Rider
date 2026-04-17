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

    void Draw();

    void Update();

};
