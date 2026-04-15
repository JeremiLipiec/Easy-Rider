#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "Simulation.h"
#include "GuiManager.h"

using namespace std;

void setup();
void draw();

Simulation* simulation;
GuiManager* gui_manager;

int main()
{
    srand(time(nullptr));

    gui_manager = GuiManager::getInstance();
    gui_manager->SetupWindow();

    setup();

    while (gui_manager->window.isOpen())
    {
        gui_manager->window.clear();
        draw();
        gui_manager->window.display();
    }
}

void setup(){
    int intersection_count = 16;
    simulation = Simulation::getInstance(intersection_count);

    //simulation->infrastructure.infrastructure_map[3][0] = 1;
    //simulation->infrastructure.infrastructure_map[1][0] = 1;
    //simulation->infrastructure.infrastructure_map[1][4] = 1;
    //simulation->infrastructure.infrastructure_map[4][7] = 1;
    //simulation->infrastructure.infrastructure_map[7][8] = 1;
    //simulation->infrastructure.infrastructure_map[8][5] = 1;

    simulation->infrastructure.FillMap();
    simulation->infrastructure.GenerateMap();

    int start_id, end_id;

    for(int i = 0; i < 40; i++){
        start_id = rand() % intersection_count;
        end_id = rand() % (intersection_count - 1);
        if(end_id >= start_id) end_id++;
        simulation->AddVehicle(2.f, 35.f, .05f, .1f, start_id, end_id);
    }
}

void draw(){
    gui_manager->Update();
    gui_manager->DrawMouseCursor();

    simulation->Update();
    simulation->Draw();
}
