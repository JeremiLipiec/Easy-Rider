#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "Traffic.h"

using namespace std;

    void Traffic::AddVehicle(float max_speed, float length, float accerleration, float breaking_force, int start_intersection_id, int finish_intersection_id){
        int delay = (int)vehicles.size() * 5;
        vehicles.push_back(Vehicle(max_speed, length, accerleration, breaking_force, start_intersection_id, finish_intersection_id));
        vehicles.back().spawn_timer = delay;
    }

    void Traffic::Update(){
        for(Vehicle& v : vehicles){
            v.Update();
        }
    }

    void Traffic::Draw(){
        for(Vehicle& v : vehicles){
            v.Draw();
        }
    }

