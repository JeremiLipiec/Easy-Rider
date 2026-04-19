#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Simulation.h"
#include "Traffic.h"

using namespace std;

    void Traffic::AddCar(int start_id, int finish_id){
        int delay = (int)vehicles.size() * 5;
        vehicles.push_back(Car(start_id, finish_id));
        vehicles.back().spawn_timer = delay;
    }

    void Traffic::AddBike(int start_id, int finish_id){
        int delay = (int)vehicles.size() * 5;
        vehicles.push_back(Bike(start_id, finish_id));
        vehicles.back().spawn_timer = delay;
    }

    void Traffic::AddBus(int start_id, int finish_id){
        int delay = (int)vehicles.size() * 5;
        vehicles.push_back(Bus(start_id, finish_id));
        vehicles.back().spawn_timer = delay;
    }

    void Traffic::Update(){
        for(int i = 0; i < vehicles.size(); i++){
            vehicles[i].Update();
            vehicles[i].CheckRemoveClick();
            if(vehicles[i].deleted){
                vehicles.erase(vehicles.begin() + i);
            }
        }
    }

    void Traffic::Draw(){
        for(Vehicle& v : vehicles){
            v.Draw();
        }
    }

