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
    int intersection_count = 25;
    simulation = Simulation::getInstance(intersection_count);

    // outer ring — two-way
    simulation->infrastructure.infrastructure_map[0][1] = 1;
    simulation->infrastructure.infrastructure_map[1][0] = 1;
    simulation->infrastructure.infrastructure_map[1][2] = 1;
    simulation->infrastructure.infrastructure_map[2][1] = 1;
    simulation->infrastructure.infrastructure_map[2][3] = 1;
    simulation->infrastructure.infrastructure_map[3][2] = 1;
    simulation->infrastructure.infrastructure_map[3][4] = 1;
    simulation->infrastructure.infrastructure_map[4][3] = 1;
    simulation->infrastructure.infrastructure_map[4][9] = 1;
    simulation->infrastructure.infrastructure_map[9][4] = 1;
    simulation->infrastructure.infrastructure_map[9][14] = 1;
    simulation->infrastructure.infrastructure_map[14][9] = 1;
    simulation->infrastructure.infrastructure_map[14][19] = 1;
    simulation->infrastructure.infrastructure_map[19][14] = 1;
    simulation->infrastructure.infrastructure_map[19][24] = 1;
    simulation->infrastructure.infrastructure_map[24][19] = 1;
    simulation->infrastructure.infrastructure_map[24][23] = 1;
    simulation->infrastructure.infrastructure_map[23][24] = 1;
    simulation->infrastructure.infrastructure_map[23][22] = 1;
    simulation->infrastructure.infrastructure_map[22][23] = 1;
    simulation->infrastructure.infrastructure_map[22][21] = 1;
    simulation->infrastructure.infrastructure_map[21][22] = 1;
    simulation->infrastructure.infrastructure_map[21][20] = 1;
    simulation->infrastructure.infrastructure_map[20][21] = 1;
    simulation->infrastructure.infrastructure_map[20][15] = 1;
    simulation->infrastructure.infrastructure_map[15][20] = 1;
    simulation->infrastructure.infrastructure_map[15][10] = 1;
    simulation->infrastructure.infrastructure_map[10][15] = 1;
    simulation->infrastructure.infrastructure_map[10][5] = 1;
    simulation->infrastructure.infrastructure_map[5][10] = 1;
    simulation->infrastructure.infrastructure_map[5][0] = 1;
    simulation->infrastructure.infrastructure_map[0][5] = 1;

    // inner ring — counter-clockwise, one-way
    simulation->infrastructure.infrastructure_map[6][11]  = 1;
    simulation->infrastructure.infrastructure_map[11][16] = 1;
    simulation->infrastructure.infrastructure_map[16][17] = 1;
    simulation->infrastructure.infrastructure_map[17][18] = 1;
    simulation->infrastructure.infrastructure_map[18][13] = 1;
    simulation->infrastructure.infrastructure_map[13][8]  = 1;
    simulation->infrastructure.infrastructure_map[8][7]   = 1;
    simulation->infrastructure.infrastructure_map[7][6]   = 1;

    // roundabout entry/exit connectors — two-way (left/right sides)
    simulation->infrastructure.infrastructure_map[10][11] = 1;
    simulation->infrastructure.infrastructure_map[11][10] = 1;
    simulation->infrastructure.infrastructure_map[14][13] = 1;
    simulation->infrastructure.infrastructure_map[13][14] = 1;
    simulation->infrastructure.infrastructure_map[22][17] = 1;
    simulation->infrastructure.infrastructure_map[17][22] = 1;
    simulation->infrastructure.infrastructure_map[2][7] = 1;
    simulation->infrastructure.infrastructure_map[7][2] = 1;

    simulation->infrastructure.infrastructure_map[11][12] = 1;
    simulation->infrastructure.infrastructure_map[12][11] = 1;

    simulation->infrastructure.GenerateMap();

    int start_id, end_id;

    for(int i = 0; i < 20; i++){
        start_id = rand() % intersection_count;
        end_id = rand() % (intersection_count - 1);
        if(end_id >= start_id) end_id++;
        simulation->traffic.AddCar(start_id, end_id);
    }
    for(int i = 0; i < 10; i++){
        start_id = rand() % intersection_count;
        end_id = rand() % (intersection_count - 1);
        if(end_id >= start_id) end_id++;
        simulation->traffic.AddBike(start_id, end_id);
    }
    for(int i = 0; i < 10; i++){
        start_id = rand() % intersection_count;
        end_id = rand() % (intersection_count - 1);
        if(end_id >= start_id) end_id++;
        simulation->traffic.AddBus(start_id, end_id);
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
