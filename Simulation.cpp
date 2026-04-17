#include <SFML/Graphics.hpp>
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

void Simulation::Draw(){
    infrastructure.Draw();
    traffic.Draw();
}

void Simulation::AddVehicle(float max_speed, float length, float accerleration, float breaking_force, int start_intersection_id, int finish_intersection_id){
    traffic.AddVehicle(max_speed, length, accerleration, breaking_force, start_intersection_id, finish_intersection_id);
}

void Simulation::Update(){
    infrastructure.Update();
    traffic.Update();
}
