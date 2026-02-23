#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class GuiManager {
private:

    static GuiManager* FInstance;

    GuiManager();

public:

    sf::Font font_object;
    sf::Text text_object;

    static GuiManager* getInstance();

    void DrawText(string text, sf::Vector2f position, sf::RenderWindow& window);
};



