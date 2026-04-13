#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "GuiManager.h"

using namespace std;

void setup();
void draw();

Simulation* simulation;
GuiManager* gui_manager;

int main()
{
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
    simulation = Simulation::getInstance(16);

    //simulation->infrastructure.infrastructure_map[3][0] = 1;
    //simulation->infrastructure.infrastructure_map[1][0] = 1;
    //simulation->infrastructure.infrastructure_map[1][4] = 1;
    //simulation->infrastructure.infrastructure_map[4][7] = 1;
    //simulation->infrastructure.infrastructure_map[7][8] = 1;
    //simulation->infrastructure.infrastructure_map[8][5] = 1;

    simulation->infrastructure.FillMap();
    simulation->infrastructure.GenerateMap();

    simulation->AddVehicle(2.f, 35.f, .1f, 1.f, 0, 1);
}

void draw(){
    gui_manager->Update();
    gui_manager->DrawMouseCursor();

    simulation->Update();
    simulation->Draw();
}
