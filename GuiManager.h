#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class GuiManager {
private:

    static GuiManager* FInstance;

    GuiManager();

public:

    short framerate = 60;
    short active_screen = 0; // 0 - main menu, 1 - simulation, 2 - repot

    sf::Font font_object;
    sf::Text text_object;

    sf::RenderWindow window;
    
    // mouse
    sf::Vector2f mouse_position;
    int mouse_size = 4;
    sf::Color mouse_color = sf::Color::White;

    bool is_dragging = false;
    sf::Vector2f drag_anchor_mouse;
    sf::Vector2f drag_anchor_origin;

    bool draw_debug = true;

    string program_name = "Easy Rider Jeremi Lipiec 348407";

    // main menu state
    string file_path_input = "";
    bool file_path_focused = false;
    sf::FloatRect mm_file_input_rect;
    sf::FloatRect mm_load_btn_rect;
    sf::FloatRect mm_start_btn_rect;

    // simulation screen state
    bool sim_paused = false;
    float sim_speed = 1.f;
    sf::FloatRect sim_pause_btn_rect;
    sf::FloatRect sim_stop_btn_rect;
    sf::FloatRect sim_speed_btn_rect;
    float speeds[5] = {0.5f, 1.f, 2.f, 5.f, 10.f};

    // report screen state
    sf::FloatRect rp_export_btn_rect;
    sf::FloatRect rp_menu_btn_rect;

    static GuiManager* getInstance();

    void SetupWindow();

    void SetupMainMenu();
    void SetupSimulation();
    void SetupReport();

    void Update();
    void Draw();

    void DrawButton(sf::FloatRect rect, const string& label, sf::Color color = sf::Color(65, 130, 200));
    void DrawMainMenu();
    void DrawSimulation();
    void DrawReport();

    void DrawDebugText(string text, sf::Vector2f position);

    void DrawMouseCursor();
};



