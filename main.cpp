#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "GuiManager.h"

using namespace std;

void setup();
void draw();

string program_name = "Easy Rider Jeremi Lipiec 348407";
sf::RenderWindow window;
sf::Vector2i mouse_position;

Simulation* simulation;
GuiManager* gui_manager;

int main()
{
    window.create(sf::VideoMode({1920, 1080}), program_name);
    window.setFramerateLimit(60);

    setup();

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        mouse_position = sf::Mouse::getPosition(window);
        window.clear();
        draw();
        window.display();
    }
}

void setup(){
    gui_manager = GuiManager::getInstance();
    simulation = Simulation::getInstance(9);

    simulation->infrastructure.infrastructure_map[3][0] = 1;
    simulation->infrastructure.infrastructure_map[3][0] = 1;
    simulation->infrastructure.infrastructure_map[1][0] = 1;
    simulation->infrastructure.infrastructure_map[1][4] = 1;

    simulation->infrastructure.GenerateMap();

    simulation->AddVehicle(2.f, 35.f, .1f, 1.f, 0, 1);
}

void draw(){
    simulation->Update();
    simulation->Draw(window);

    sf::CircleShape shape(5.f);
    shape.setPosition(sf::Vector2f(mouse_position.x - 5.f, mouse_position.y - 5.f));
    shape.setFillColor(sf::Color::Blue);
    window.draw(shape);
}
