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

    for (size_t i = 0; i < 2; i++)
    {
        int random_start_intersection = rand() % simulation->infrastructure.intersection_count;
        int random_end_intersection = rand() % simulation->infrastructure.intersection_count;
        simulation->traffic.AddCar(random_start_intersection, random_end_intersection);
    }
}

void draw()
{
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

    gui_manager->Draw();
    gui_manager->Update();
}
