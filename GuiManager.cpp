#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "GuiManager.h"
#include "Simulation.h"

static string OpenFileDialog()
{
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "EasyRider Map\0*.ezrdr\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrDefExt = "ezrdr";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    return GetOpenFileNameA(&ofn) ? string(filename) : "";
}

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
    window.create(sf::VideoMode({1920, 1080}), "Easy Rider Jeremi Lipiec 348407");
    window.setFramerateLimit(framerate);
    active_screen = 0;
}

void GuiManager::DrawDebugText(string text, sf::Vector2f position)
{
    if (!GuiManager::getInstance()->draw_debug)
        return;

    text_object.setString(text);
    text_object.setPosition(position);
    window.draw(text_object);
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

    // info box header
    sf::Text info_header(font_object);
    info_header.setCharacterSize(22);
    info_header.setFillColor(sf::Color::White);
    info_header.setString("About the program");
    info_header.setPosition({box_x + 24.f, box_y + 16.f});
    window.draw(info_header);

    // info lines
    sf::Text info_credit(font_object);
    info_credit.setCharacterSize(16);
    info_credit.setFillColor(sf::Color(160, 170, 190));
    info_credit.setString("Created by Jeremi Lipiec for the Advanced C++ course.");
    info_credit.setPosition({box_x + 24.f, box_y + 48.f});
    window.draw(info_credit);

    vector<pair<string, string>> info_lines = {
        {"Traffic simulation", "Simulate road traffic in real time. Vehicles find the shortest path between intersections using Dijkstra's algorithm and respect traffic lights and one-way roads."},
        {"Loading map files", "Type a file path or click 'Load file' to open a file browser and select a .ezrdr graph file. The simulation reloads instantly with the new road network."},
        {"Creating map files", "Map files are plain-text adjacency matrices in CSV format. Each row i and column j holds 1 if a road exists from intersection i to j, or 0 otherwise. One-way roads are encoded by setting only one direction to 1."},
        {"Spawning vehicles", "During simulation, click any intersection on the map to instantly spawn a random vehicle (car, bike or bus) there. It will immediately find a path to a random destination and join the traffic."},
    };

    float line_y = box_y + 82.f;
    for (auto &[heading, body] : info_lines)
    {
        sf::Text h(font_object);
        h.setCharacterSize(18);
        h.setFillColor(sf::Color(100, 180, 255));
        h.setString("- " + heading + ":");
        h.setPosition({box_x + 24.f, line_y});
        window.draw(h);
        line_y += 26.f;

        sf::Text b(font_object);
        b.setCharacterSize(16);
        b.setFillColor(sf::Color(200, 210, 225));
        b.setString(body);
        b.setPosition({box_x + 40.f, line_y});
        window.draw(b);
        line_y += 34.f;
    }

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
    input_text.setString(file_path_input.empty() ? "./graphs/graph.ezrdr" : file_path_input);
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
    string speed_label = (sim_speed == speeds[0]) ? "x0.5" : (sim_speed == speeds[1]) ? "x1"
                                                         : (sim_speed == speeds[2])   ? "x2"
                                                         : (sim_speed == speeds[3])   ? "x5"
                                                         : (sim_speed == speeds[4])   ? "x10"
                                                                                      : "";
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
    auto *sim = Simulation::getInstance();
    auto &traffic = sim->traffic;

    sf::Vector2f win(window.getSize());

    sf::RectangleShape bg(win);
    bg.setFillColor(sf::Color(28, 38, 56));
    window.draw(bg);

    // title
    sf::Text title(font_object);
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setString("Simulation report");
    title.setPosition({60.f, 40.f});
    window.draw(title);

    // report box
    float box_x = 60.f, box_y = 100.f;
    float box_w = win.x - 120.f, box_h = win.y - 200.f;
    sf::RectangleShape box({box_w, box_h});
    box.setPosition({box_x, box_y});
    box.setFillColor(sf::Color(40, 52, 72));
    window.draw(box);

    // compute stats
    int total = (int)traffic.vehicles.size();
    int active = 0;
    float avg_spd = 0.f;
    for (auto &v : traffic.vehicles)
        if (v.is_spawned)
        {
            active++;
            avg_spd += v.speed;
        }
    if (active > 0)
        avg_spd /= active;

    int ticks = sim->sim_ticks;
    int secs = ticks / 60;
    int mins = secs / 60;
    secs %= 60;

    float py = box_y + 24.f;
    float px = box_x + 30.f;

    auto draw_row = [&](const string &label, const string &value)
    {
        sf::Text lbl(font_object);
        lbl.setCharacterSize(20);
        lbl.setFillColor(sf::Color(160, 180, 210));
        lbl.setString(label);
        lbl.setPosition({px, py});
        window.draw(lbl);

        sf::Text val(font_object);
        val.setCharacterSize(20);
        val.setFillColor(sf::Color::White);
        val.setString(value);
        sf::FloatRect vb = val.getLocalBounds();
        val.setPosition({box_x + box_w - 30.f - vb.size.x - vb.position.x, py});
        window.draw(val);

        py += 36.f;

        sf::RectangleShape sep({box_w - 60.f, 1.f});
        sep.setPosition({px, py - 8.f});
        sep.setFillColor(sf::Color(60, 75, 100));
        window.draw(sep);
    };

    draw_row("Simulation time",
             (mins > 0 ? to_string(mins) + " min " : "") + to_string(secs) + " sec  (" + to_string(ticks) + " ticks)");
    draw_row("Vehicles in simulation", to_string(total));
    draw_row("Average speed", to_string(avg_spd).substr(0, 4));
    draw_row("Completed journeys", to_string(traffic.completed_journeys));

    // bottom buttons
    float btn_w = 180.f, btn_h = 44.f;
    float btn_y = box_y + box_h + 20.f;
    rp_export_btn_rect = {{box_x, btn_y}, {btn_w, btn_h}};
    rp_menu_btn_rect = {{box_x + box_w - btn_w, btn_y}, {btn_w, btn_h}};
    DrawButton(rp_export_btn_rect, "Export report", sf::Color(50, 130, 80));
    DrawButton(rp_menu_btn_rect, "Main menu", sf::Color(65, 130, 200));
}

void GuiManager::Draw()
{
    DrawMouseCursor();

    switch (active_screen)
    {
    case 0:
        DrawMainMenu();
        break;
    case 1:
        DrawSimulation();
        break;
    case 2:
        DrawReport();
        break;
    default:
        break;
    }
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

        // simulation logic
        if (active_screen == 0)
        {
            if (const auto *clicked = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (clicked->button == sf::Mouse::Button::Left)
                {
                    file_path_focused = mm_file_input_rect.contains(mouse_position);

                    if (mm_load_btn_rect.contains(mouse_position))
                    {
                        string picked = OpenFileDialog();
                        if (!picked.empty())
                        {
                            file_path_input = picked;
                            Simulation::getInstance()->LoadMap(file_path_input);
                            Simulation::getInstance()->InitTraffic();
                        }
                    }

                    if (mm_start_btn_rect.contains(mouse_position))
                        active_screen = 1;
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

        // main menu logic
        if (active_screen == 1)
        {
            if (const auto *clicked = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (clicked->button == sf::Mouse::Button::Left)
                {
                    if (sim_pause_btn_rect.contains(mouse_position))
                        sim_paused = !sim_paused;
                    if (sim_stop_btn_rect.contains(mouse_position))
                        active_screen = 2;
                    if (sim_speed_btn_rect.contains(mouse_position))
                    {
                        if (sim_speed == speeds[0])
                            sim_speed = speeds[1];
                        else if (sim_speed == speeds[1])
                            sim_speed = speeds[2];
                        else if (sim_speed == speeds[2])
                            sim_speed = speeds[3];
                        else if (sim_speed == speeds[3])
                            sim_speed = speeds[4];
                        else
                            sim_speed = speeds[0];
                    }
                }
            }

            if (const auto *released = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (released->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f delta = mouse_position - drag_anchor_mouse;
                    bool is_click = (delta.x * delta.x + delta.y * delta.y) < 64.f; // < 8px movement
                    if (is_click && mouse_position.x > 220.f)
                    {
                        auto &infra = Simulation::getInstance()->infrastructure;
                        auto &traffic = Simulation::getInstance()->traffic;
                        int n = infra.intersection_count;
                        for (auto &i : infra.intersections)
                        {
                            if (i.used && i.boundingBox.contains(mouse_position))
                            {
                                int end_id = rand() % (n - 1);
                                if (end_id >= i.id)
                                    end_id++;
                                switch (rand() % 3)
                                {
                                case 0:
                                    traffic.AddCar(i.id, end_id);
                                    break;
                                case 1:
                                    traffic.AddBike(i.id, end_id);
                                    break;
                                case 2:
                                    traffic.AddBus(i.id, end_id);
                                    break;
                                }
                                traffic.vehicles.back().spawn_timer = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }

        // report screen logic
        if (active_screen == 2)
        {
            if (const auto *clicked = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (clicked->button == sf::Mouse::Button::Left)
                {
                    if (rp_menu_btn_rect.contains(mouse_position))
                        active_screen = 0;

                    if (rp_export_btn_rect.contains(mouse_position))
                    {
                        auto *sim = Simulation::getInstance();
                        auto &traffic = sim->traffic;
                        int total = (int)traffic.vehicles.size();
                        int active = 0;
                        float avg_spd = 0.f;
                        for (auto &v : traffic.vehicles)
                            if (v.is_spawned)
                            {
                                active++;
                                avg_spd += v.speed;
                            }
                        if (active > 0)
                            avg_spd /= active;
                        int ticks = sim->sim_ticks;
                        int secs = ticks / framerate, mins = secs / framerate;
                        secs %= framerate;

                        ofstream f("report.txt");
                        if (f.is_open())
                        {
                            f << "=== Easy Rider Simulation Report ===\n\n";
                            f << "Simulation time: " << mins << " min " << secs << " sec (" << ticks << " ticks)\n";
                            f << "Vehicles in simulation: " << total << "\n";
                            f << "Average speed: " << avg_spd << "\n";
                            f << "Completed journeys: " << traffic.completed_journeys << "\n";
                        }
                    }
                }
            }
        }
    }
}

void GuiManager::DrawMouseCursor()
{
    sf::CircleShape shape(mouse_size);
    shape.setPosition(sf::Vector2f(mouse_position.x - mouse_size, mouse_position.y - mouse_size));
    shape.setFillColor(mouse_color);
    window.draw(shape);
}
