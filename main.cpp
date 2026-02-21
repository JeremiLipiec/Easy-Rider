#include <SFML/Graphics.hpp>
#include "Infrastructure.h"

using namespace std;

void setup();
void draw();

string program_name = "Easy Rider Jeremi Lipiec 348407";
sf::RenderWindow window;
sf::Vector2i mouse_position;
int x = 0;
Infrastructure infrastructure(9);


void init_window(unsigned int size_x, unsigned int size_y){
    window.create(sf::VideoMode({size_x, size_y}), program_name);
}

int main()
{
    init_window(1920, 1080);
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
    infrastructure.infrastructure_map[3][0] = 1;
    infrastructure.generate_map();

}

void draw(){
    infrastructure.draw_map(window);

    sf::CircleShape shape(5.f);
    shape.setPosition(sf::Vector2f(mouse_position.x - 5.f, mouse_position.y - 5.f));
    shape.setFillColor(sf::Color::Green);
    window.draw(shape);
}
