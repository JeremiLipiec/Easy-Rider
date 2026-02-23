#include <SFML/Graphics.hpp>
#include "GuiManager.h"

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

void GuiManager::DrawText(string text, sf::Vector2f position, sf::RenderWindow& window){
    text_object.setString(text);
    text_object.setPosition(position);
    window.draw(text_object);
}
