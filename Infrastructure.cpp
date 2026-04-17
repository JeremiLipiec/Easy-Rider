#include <SFML/Graphics.hpp>
#include <cmath>
#include "Infrastructure.h"
#include "GuiManager.h"
#include "Simulation.h"

using namespace std;

Infrastructure::Infrastructure() {}

void Infrastructure::Setup(int _intersection_count)
{
    intersection_count = _intersection_count;
    map_size = sqrt(intersection_count);
    infrastructure_map = vector<vector<int>>(intersection_count, vector<int>(intersection_count, 0));
}


void Infrastructure::GenerateMap()
{
    int intersection_id = 0;

    for (int y = 0; y < map_size; y++)
    {
        for (int x = 0; x < map_size; x++)
        {
            sf::Vector2f local_pos(intersection_distance * (intersection_id % map_size), intersection_distance * (intersection_id / map_size));
            intersections.push_back(Intersection(intersection_id++, local_pos, true));
        }
    }

    for (int y = 0; y < intersection_count; y++)
    {
        for (int x = 0; x < intersection_count; x++)
        {
            if (infrastructure_map[x][y] == 1 && x != y)
            {
                roads.push_back(Road(intersections[x].id, intersections[y].id, 50));
                intersections[x].used = true;
                intersections[y].used = true;
            }
        }
    }
}

void Infrastructure::UpdatePositions()
{
    for (Intersection &i : intersections)
        i.UpdateGlobalPosition();
    for (Road &r : roads)
        r.UpdateGlobalPosition();
}

void Infrastructure::Update()
{
    UpdatePositions();

    for (Intersection &i : intersections)
        i.Update();
}

void Infrastructure::Draw()
{
    for (Road &r : roads)
    {
        r.Draw();
    }

    for (Intersection &i : intersections)
    {
        i.Draw();
    }
}
