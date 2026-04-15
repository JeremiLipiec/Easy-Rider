#include <SFML/Graphics.hpp>
#include "GuiManager.h"
#include "Simulation.h"

using namespace std;

GuiManager* GuiManager::FInstance = nullptr;

GuiManager::GuiManager() : text_object(font_object) {
    font_object.openFromFile("C:/Windows/Fonts/arial.ttf");
    text_object.setCharacterSize(24);
    text_object.setFillColor(sf::Color::Cyan);
}

GuiManager* GuiManager::getInstance() {
    if (!FInstance) {
        FInstance = new GuiManager();
    }
    return FInstance;
}

void GuiManager::SetupWindow(){
    window.create(sf::VideoMode({1920, 1080}), "Easy Rider Jeremi Lipiec 348407");
    window.setFramerateLimit(60);
}

void GuiManager::DrawText(string text, sf::Vector2f position){
    if(!GuiManager::getInstance()->draw_debug)
        return;

    text_object.setString(text);
    text_object.setPosition(position);
    window.draw(text_object);
}

void GuiManager::Update(){
    mouse_position = sf::Mouse::getPosition(window);

    while (const optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void GuiManager::DrawMouseCursor(){
    sf::CircleShape shape(5.f);
    shape.setPosition(sf::Vector2f(mouse_position.x - 5.f, mouse_position.y - 5.f));
    shape.setFillColor(sf::Color::Blue);
    window.draw(shape);
}
