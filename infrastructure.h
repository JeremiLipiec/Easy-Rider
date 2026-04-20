#pragma once

#include <SFML/Graphics.hpp>
#include "Road.h"
#include "Intersection.h"
#include <cmath>

using namespace std;

class Simulation;

class Infrastructure {
public:

    sf::Vector2f drawing_origin = {100.f, 100.f};
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

    void UpdatePositions(); // call after changing drawing_origin to move the entire infrastructure

    void Update();

    void GenerateMap();

    void Draw();
};

