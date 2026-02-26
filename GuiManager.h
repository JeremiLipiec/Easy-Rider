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

    sf::RenderWindow window;
    sf::Vector2i mouse_position;

    string program_name = "Easy Rider Jeremi Lipiec 348407";

    static GuiManager* getInstance();

    void SetupWindow();

    void Update();

    void DrawText(string text, sf::Vector2f position);

    void DrawMouseCursor();
};



