#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include "Simulation.h"
#include "GuiManager.h"

using namespace std;

Simulation* Simulation::FInstance = nullptr;

Simulation* Simulation::getInstance(int _intersection_count) {
    if (!FInstance) {
        FInstance = new Simulation(_intersection_count);
    }
    return FInstance;
}

Simulation::Simulation(int _intersection_count) {
    infrastructure = Infrastructure();
    infrastructure.Setup(_intersection_count);
}

bool Simulation::LoadMap(const string& path)
{
    ifstream file(path);
    if (!file.is_open())
        return false;

    vector<vector<int>> rows;
    string line;
    while (getline(file, line))
    {
        vector<int> row;
        stringstream ss(line);
        string cell;
        while (getline(ss, cell, ','))
            row.push_back(stoi(cell));
        if (!row.empty())
            rows.push_back(row);
    }

    int n = (int)rows.size();
    infrastructure.Setup(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n && j < (int)rows[i].size(); j++)
            infrastructure.infrastructure_map[i][j] = rows[i][j];

    infrastructure.GenerateMap();
    return true;
}

void Simulation::InitTraffic()
{
    traffic.vehicles.clear();
    int n = infrastructure.intersection_count;
    int start_id, end_id;

    for (int i = 0; i < 20; i++) {
        start_id = rand() % n;
        end_id = rand() % (n - 1);
        if (end_id >= start_id) end_id++;
        traffic.AddCar(start_id, end_id);
    }
    for (int i = 0; i < 10; i++) {
        start_id = rand() % n;
        end_id = rand() % (n - 1);
        if (end_id >= start_id) end_id++;
        traffic.AddBike(start_id, end_id);
    }
    for (int i = 0; i < 10; i++) {
        start_id = rand() % n;
        end_id = rand() % (n - 1);
        if (end_id >= start_id) end_id++;
        traffic.AddBus(start_id, end_id);
    }
}

void Simulation::Draw(){
    infrastructure.Draw();
    traffic.Draw();
}

void Simulation::Update(){
    infrastructure.Update();
    traffic.Update();
}
