#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "Simulation.h"
#include "GuiManager.h"

using namespace std;

void setup();
void draw();

Simulation *simulation;
GuiManager *gui_manager;

static int half_frame = 0;

int main()
{
    srand(time(nullptr));

    gui_manager = GuiManager::getInstance();
    

    setup();

    while (gui_manager->window.isOpen())
    {
        gui_manager->window.clear();
        draw();
        gui_manager->window.display();
    }
}

void setup()
{
    gui_manager->SetupWindow();

    simulation = Simulation::getInstance(1);
    simulation->LoadMap("graphs/graph.ezrdr");
    simulation->InitTraffic();
}

void draw()
{
    gui_manager->Draw();
    gui_manager->Update();

    if (gui_manager->active_screen == 1)
    {
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
    }
}
