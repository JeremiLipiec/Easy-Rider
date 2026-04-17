#pragma once

#include <SFML/Graphics.hpp>
#include "Road.h"
#include "Intersection.h"
#include <cmath>

using namespace std;

class Simulation;

class Infrastructure {
public:

    float drawing_start_x = 100.f;
    float drawing_start_y = 100.f;
    float intersection_distance = 300.f;
    float road_thickness = 70.f;
    float intersection_size = 70.f;

    int intersection_count; // 4
    int map_size;

    vector<vector<int>> infrastructure_map; // macierz sasiedztwa
    vector<Road> roads;
    vector<Intersection> intersections;

    Infrastructure();

    void Setup(int _intersection_count);

    void Update();

    void GenerateMap();

    void FillMap();

    void Draw();
};



