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

    simulation->infrastructure.infrastructure_map[0][1] = 1;
    simulation->infrastructure.infrastructure_map[1][3] = 1;

    simulation->infrastructure.FillMap();
    simulation->infrastructure.GenerateMap();

    int start_id, end_id;

    for(int i = 0; i < 40; i++){
        start_id = rand() % intersection_count;
        end_id = rand() % (intersection_count - 1);
        if(end_id >= start_id) end_id++;
        simulation->AddVehicle(2.f, 35.f, .05f, .1f, start_id, end_id);
    }

    gui_manager->SetupMainMenu();
}

void draw(){
    gui_manager->Update();

    switch (gui_manager->active_screen)
    {
        case 0:
            gui_manager->DrawMainMenu();
            break;
        case 1:
        {
            static int half_frame = 0;
            if (!gui_manager->sim_paused)
            {
                bool run = (gui_manager->sim_speed == 0.5f) ? (half_frame++ % 2 == 0) : true;
                int ticks = (gui_manager->sim_speed >= 1.f) ? (int)gui_manager->sim_speed : 1;
                if (run)
                    for (int t = 0; t < ticks; t++)
                        simulation->Update();
            }
            else
                simulation->infrastructure.UpdatePositions();
            simulation->Draw();
            gui_manager->DrawSimulation();
            break;
        }
        case 2:
            gui_manager->DrawReport();
            break;
    }

    gui_manager->DrawMouseCursor();
}
