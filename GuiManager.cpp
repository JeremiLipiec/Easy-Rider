#include <SFML/Graphics.hpp>
#include "GuiManager.h"
#include "Simulation.h"

using namespace std;

GuiManager *GuiManager::FInstance = nullptr;

GuiManager::GuiManager() : text_object(font_object)
{
    font_object.openFromFile("C:/Windows/Fonts/arial.ttf");
    text_object.setCharacterSize(24);
    text_object.setFillColor(sf::Color::Cyan);
}

GuiManager *GuiManager::getInstance()
{
    if (!FInstance)
    {
        FInstance = new GuiManager();
    }
    return FInstance;
}

void GuiManager::SetupWindow()
{
    window.create(sf::VideoMode({1000, 1000}), "Easy Rider Jeremi Lipiec 348407");
    window.setFramerateLimit(60);
}

void GuiManager::DrawText(string text, sf::Vector2f position)
{
    if (!GuiManager::getInstance()->draw_debug)
        return;

    text_object.setString(text);
    text_object.setPosition(position);
    window.draw(text_object);
}

void GuiManager::SetupMainMenu()
{
    active_screen = 0;
}

void GuiManager::SetupSimulation()
{
    active_screen = 1;
}

void GuiManager::SetupReport()
{
    active_screen = 2;
}

void GuiManager::DrawButton(sf::FloatRect rect, const string &label, sf::Color color)
{
    sf::RectangleShape btn(rect.size);
    btn.setPosition(rect.position);
    btn.setFillColor(color);
    window.draw(btn);

    sf::Text txt(font_object);
    txt.setCharacterSize(20);
    txt.setFillColor(sf::Color::White);
    txt.setString(label);
    sf::FloatRect tb = txt.getLocalBounds();
    txt.setPosition({rect.position.x + (rect.size.x - tb.size.x) / 2.f - tb.position.x, rect.position.y + (rect.size.y - tb.size.y) / 2.f - tb.position.y});
    window.draw(txt);
}

void GuiManager::DrawMainMenu()
{
    sf::Vector2f win(window.getSize());

    // background
    sf::RectangleShape bg(win);
    bg.setFillColor(sf::Color(28, 38, 56));
    window.draw(bg);

    // title
    sf::Text title(font_object);
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);
    title.setString("Easy Rider");
    title.setPosition({60.f, 40.f});
    window.draw(title);

    sf::Text subtitle(font_object);
    subtitle.setCharacterSize(22);
    subtitle.setFillColor(sf::Color(180, 190, 210));
    subtitle.setString("Jeremi Lipiec 348407");
    subtitle.setPosition({60.f, 100.f});
    window.draw(subtitle);

    // info box
    float box_w = win.x - 120.f;
    float box_h = win.y * 0.52f;
    float box_x = 60.f;
    float box_y = 145.f;
    sf::RectangleShape info_box({box_w, box_h});
    info_box.setPosition({box_x, box_y});
    info_box.setFillColor(sf::Color(40, 52, 72));
    window.draw(info_box);

    sf::Text info_label(font_object);
    info_label.setCharacterSize(20);
    info_label.setFillColor(sf::Color(160, 170, 190));
    info_label.setString("Program information and usage instruction");
    sf::FloatRect ilb = info_label.getLocalBounds();
    info_label.setPosition({box_x + (box_w - ilb.size.x) / 2.f - ilb.position.x,
                            box_y + (box_h - ilb.size.y) / 2.f - ilb.position.y});
    window.draw(info_label);

    // infrastructure graph section
    float section_y = box_y + box_h + 40.f;

    sf::Text graph_label(font_object);
    graph_label.setCharacterSize(20);
    graph_label.setFillColor(sf::Color(180, 190, 210));
    graph_label.setString("Infrastructure graph");
    graph_label.setPosition({box_x, section_y});
    window.draw(graph_label);

    float input_y = section_y + 36.f;
    float input_w = 500.f;
    float btn_w = 120.f;
    float field_h = 40.f;

    mm_file_input_rect = {{box_x, input_y}, {input_w, field_h}};
    mm_load_btn_rect = {{box_x + input_w + 10.f, input_y}, {btn_w, field_h}};

    // file path input
    sf::RectangleShape input_box(mm_file_input_rect.size);
    input_box.setPosition(mm_file_input_rect.position);
    input_box.setFillColor(sf::Color(50, 62, 82));
    input_box.setOutlineThickness(2.f);
    input_box.setOutlineColor(file_path_focused ? sf::Color(65, 130, 200) : sf::Color(80, 95, 120));
    window.draw(input_box);

    sf::Text input_text(font_object);
    input_text.setCharacterSize(18);
    input_text.setFillColor(file_path_input.empty() ? sf::Color(100, 115, 140) : sf::Color::White);
    input_text.setString(file_path_input.empty() ? "File path..." : file_path_input);
    input_text.setPosition({box_x + 10.f, input_y + 10.f});
    window.draw(input_text);

    DrawButton(mm_load_btn_rect, "Load file");

    // start simulation button
    float start_w = 200.f;
    float start_h = 44.f;
    mm_start_btn_rect = {{(win.x - start_w) / 2.f, input_y + field_h + 30.f}, {start_w, start_h}};
    DrawButton(mm_start_btn_rect, "Start simulation");
}

void GuiManager::DrawSimulation()
{
    auto &traffic = Simulation::getInstance()->traffic;

    // stats
    int total = (int)traffic.vehicles.size();
    int active = 0;
    float avg_spd = 0.f;
    for (auto &v : traffic.vehicles)
    {
        if (v.is_spawned)
        {
            active++;
            avg_spd += v.speed;
        }
    }
    if (active > 0)
        avg_spd /= active;

    sf::Vector2f win(window.getSize());

    // left panel
    float panel_w = 220.f;
    sf::RectangleShape panel({panel_w, win.y});
    panel.setPosition({0.f, 0.f});
    panel.setFillColor(sf::Color(22, 32, 50, 220));
    window.draw(panel);

    float px = 16.f, py = 16.f;

    sf::Text sim_title(font_object);
    sim_title.setCharacterSize(26);
    sim_title.setFillColor(sf::Color::White);
    sim_title.setString("Simulation");
    sim_title.setPosition({px, py});
    window.draw(sim_title);
    py += 44.f;

    // play button
    sim_pause_btn_rect = {{px, py}, {44.f, 36.f}};
    DrawButton(sim_pause_btn_rect, sim_paused ? ">" : "||", sim_paused ? sf::Color(60, 160, 60) : sf::Color(200, 120, 30));

    // speed button
    sim_speed_btn_rect = {{px + 54.f, py}, {44.f, 36.f}};
    string speed_label = (sim_speed == speeds[0]) ? "x0.5" : (sim_speed == speeds[1]) ? "x1" : (sim_speed == speeds[2]) ? "x2" : (sim_speed == speeds[3]) ? "x5" : "";
    DrawButton(sim_speed_btn_rect, speed_label, sf::Color(80, 80, 200));

    // stop button
    sim_stop_btn_rect = {{px + 110.f, py}, {44.f, 36.f}};
    DrawButton(sim_stop_btn_rect, "", sf::Color(180, 40, 40));
    sf::RectangleShape stop_icon({14.f, 14.f});
    stop_icon.setPosition({sim_stop_btn_rect.position.x + 15.f, sim_stop_btn_rect.position.y + 11.f});
    stop_icon.setFillColor(sf::Color::White);
    window.draw(stop_icon);
    py += 56.f;

    // stats
    sf::Text stats_title(font_object);
    stats_title.setCharacterSize(20);
    stats_title.setFillColor(sf::Color(180, 190, 210));
    stats_title.setString("Statistics");
    stats_title.setPosition({px, py});
    window.draw(stats_title);
    py += 32.f;

    auto draw_stat = [&](const string &label, const string &value)
    {
        sf::Text t(font_object);
        t.setCharacterSize(17);
        t.setFillColor(sf::Color(210, 220, 235));
        t.setString(label + ": " + value);
        t.setPosition({px, py});
        window.draw(t);
        py += 26.f;
    };

    draw_stat("Vehicles total", to_string(total));
    draw_stat("Vehicles active", to_string(active));
    draw_stat("Avg speed", to_string((int)(avg_spd * 10) / 10.f).substr(0, 4));
}

void GuiManager::DrawReport()
{
    sf::Vector2f win(window.getSize());

    sf::RectangleShape bg(win);
    bg.setFillColor(sf::Color(28, 38, 56));
    window.draw(bg);

    sf::Text title(font_object);
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setString("Simulation report");
    title.setPosition({60.f, 40.f});
    window.draw(title);
}

void GuiManager::Update()
{
    mouse_position = sf::Vector2f(sf::Mouse::getPosition(window));

    // drag only on simulation screen
    if (active_screen == 1)
    {
        auto &origin = Simulation::getInstance()->infrastructure.drawing_origin;
        bool mouse_down = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        if (mouse_down && !is_dragging)
        {
            is_dragging = true;
            drag_anchor_mouse = mouse_position;
            drag_anchor_origin = origin;
        }
        if (!mouse_down)
            is_dragging = false;
        if (is_dragging)
            origin = drag_anchor_origin + sf::Vector2f(mouse_position - drag_anchor_mouse);
    }

    while (const optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto *resized = event->getIf<sf::Event::Resized>())
            window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));

        if (active_screen == 1)
        {
            if (const auto *clicked = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (clicked->button == sf::Mouse::Button::Left)
                {
                    if (sim_pause_btn_rect.contains(mouse_position))
                        sim_paused = !sim_paused;
                    if (sim_stop_btn_rect.contains(mouse_position))
                        SetupMainMenu();
                    if (sim_speed_btn_rect.contains(mouse_position)){
                        if(sim_speed == speeds[0]) sim_speed = speeds[1];
                        else if(sim_speed == speeds[1]) sim_speed = speeds[2];
                        else if(sim_speed == speeds[2]) sim_speed = speeds[3];
                        else sim_speed = speeds[0];
                    }
                }
            }
        }

        if (active_screen == 0)
        {
            if (const auto *clicked = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (clicked->button == sf::Mouse::Button::Left)
                {
                    file_path_focused = mm_file_input_rect.contains(mouse_position);

                    if (mm_load_btn_rect.contains(mouse_position))
                    {
                        // TODO: load infrastructure from file_path_input
                    }

                    if (mm_start_btn_rect.contains(mouse_position))
                        SetupSimulation();
                }
            }

            if (const auto *typed = event->getIf<sf::Event::TextEntered>())
            {
                if (file_path_focused)
                {
                    if (typed->unicode == '\b' && !file_path_input.empty())
                        file_path_input.pop_back();
                    else if (typed->unicode >= 32 && typed->unicode < 128)
                        file_path_input += static_cast<char>(typed->unicode);
                }
            }
        }
    }
}

void GuiManager::DrawMouseCursor()
{
    sf::CircleShape shape(5.f);
    shape.setPosition(sf::Vector2f(mouse_position.x - 5.f, mouse_position.y - 5.f));
    shape.setFillColor(sf::Color::Blue);
    window.draw(shape);
}
