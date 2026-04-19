#include <SFML/Graphics.hpp>
#include <queue>
#include <limits>
#include <algorithm>
#include "Vehicle.h"
#include "Simulation.h"
#include "Intersection.h"
#include "GuiManager.h"

using namespace std;

Vehicle::Vehicle(float _max_speed, float _car_length, float _car_width, float _accerleration, float _breaking_force, int _start_intersection_id, int _finish_intersection_id)
{
    max_speed = _max_speed;
    car_length = _car_length;
    car_width = _car_width;
    accerleration = _accerleration;
    breaking_force = _breaking_force;
    start_intersection_id = _start_intersection_id;
    finish_intersection_id = _finish_intersection_id;
}

void Vehicle::Setup()
{
    speed = 0;
    path = CalculatePath();
    if (path.size() < 2)
        return;
    AdvanceToNextIntersection();

    Intersection &current = Simulation::getInstance()->infrastructure.intersections[current_intersection_id];
    Intersection &next = Simulation::getInstance()->infrastructure.intersections[next_intersection_id];

    sf::Vector2f dir = next.local_position - current.local_position;
    dir = dir / dir.length();
    sf::Vector2f right = {-dir.y, dir.x};

    auto &infra = Simulation::getInstance()->infrastructure;
    moving_angle = dir.angle();
    position = current.local_position
             + dir   * (infra.intersection_size / 2.f + 1.f)
             + right * (infra.road_thickness / 4.f);
}


vector<int> Vehicle::CalculatePath()
{
    auto &infra = Simulation::getInstance()->infrastructure;
    int n = infra.intersection_count;

    vector<float> dist(n, 99999999);
    vector<int> came_from(n, -1);
    vector<bool> visited(n, false);

    dist[start_intersection_id] = 0.f;

    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    pq.push({0.f, start_intersection_id});

    while (!pq.empty())
    {
        auto [d, current] = pq.top();
        pq.pop();

        if (visited[current])
            continue;
        visited[current] = true;

        if (current == finish_intersection_id)
            break;

        for (int next = 0; next < n; next++)
        {
            if (infra.infrastructure_map[current][next] != 1 || visited[next])
                continue;

            float new_dist = dist[current] + 1.f;

            if (new_dist < dist[next])
            {
                came_from[next] = current;
                dist[next] = new_dist;
                pq.push({new_dist, next});
            }
        }
    }

    vector<int> result_path;
    if (dist[finish_intersection_id] == 99999999)
        return result_path;

    for (int cur = finish_intersection_id; cur != -1; cur = came_from[cur])
        result_path.push_back(cur);

    reverse(result_path.begin(), result_path.end());
    return result_path;
}

void Vehicle::AdvanceToNextIntersection()
{
    current_intersection_id = path[0];
    next_intersection_id = path[1];
    turning_direction = CalculateTurningDirection();

    if (path.size() > 1)
        path.erase(path.begin());

    // heading to last intersection — recalculate now so turning_direction is correct on arrival
    if (path.size() == 1)
    {
        auto &infra = Simulation::getInstance()->infrastructure;
        int prev = current_intersection_id;
        start_intersection_id = next_intersection_id;
        finish_intersection_id = rand() % (infra.intersection_count - 1);
        if (finish_intersection_id >= start_intersection_id)
            finish_intersection_id++;
        Simulation::getInstance()->traffic.completed_journeys++;

        int original = infra.infrastructure_map[start_intersection_id][prev];
        infra.infrastructure_map[start_intersection_id][prev] = 0;
        vector<int> new_path = CalculatePath();
        infra.infrastructure_map[start_intersection_id][prev] = original;

        if (new_path.size() < 2)
        {
            // no forward path — try traveling backward via a predecessor
            int back = -1;
            for (int j = 0; j < infra.intersection_count; j++)
            {
                if (j != prev && j != start_intersection_id &&
                    infra.infrastructure_map[j][start_intersection_id] == 1)
                {
                    back = j;
                    break;
                }
            }
            if (back >= 0)
                new_path = {start_intersection_id, back};
            else if (prev >= 0)
                new_path = {start_intersection_id, prev};
        }

        if (new_path.size() >= 2)
        {
            path = new_path;

            sf::Vector2f v1 = infra.intersections[next_intersection_id].position - infra.intersections[current_intersection_id].position;
            sf::Vector2f v2 = infra.intersections[path[1]].position - infra.intersections[next_intersection_id].position;
            float len1 = v1.length(), len2 = v2.length();
            if (len1 > 0.001f && len2 > 0.001f)
            {
                v1 /= len1; v2 /= len2;
                float dot = v1.x * v2.x + v1.y * v2.y;
                if (dot < -0.5f)
                    turning_direction = 3;
                else
                {
                    float cross = v1.x * v2.y - v1.y * v2.x;
                    if (cross > 0.f) turning_direction = 2;
                    else if (cross < 0.f) turning_direction = 0;
                    else turning_direction = 1;
                }
            }
        }
    }
}

int Vehicle::CalculateTurningDirection()
{
    if (path.size() < 3)
        return -1;

    Intersection &current = Simulation::getInstance()->infrastructure.intersections[current_intersection_id];
    Intersection &next = Simulation::getInstance()->infrastructure.intersections[next_intersection_id];
    Intersection &after_next = Simulation::getInstance()->infrastructure.intersections[path[2]];

    sf::Vector2f v1 = next.position - current.position;
    sf::Vector2f v2 = after_next.position - next.position;

    float len1 = v1.length();
    float len2 = v2.length();
    if (len1 < 0.001f || len2 < 0.001f)
        return 1;

    v1 = v1 / len1;
    v2 = v2 / len2;

    float dot = v1.x * v2.x + v1.y * v2.y;
    if (dot < -0.5f)
        return 3; // U-turn

    float cross = v1.x * v2.y - v1.y * v2.x;
    if (cross > 0.f)
        return 2; // RIGHT
    if (cross < 0.f)
        return 0; // LEFT
    return 1;     // STRAIGHT
}

void Vehicle::Update()
{
    if (!is_spawned)
    {
        if (spawn_timer-- > 0)
            return;
        Setup();
        if (path.empty())
            return;
        is_spawned = true;
        sf::Vector2f forward = {std::cos(moving_angle.asRadians()), std::sin(moving_angle.asRadians())};
        sf::Vector2f gpos = position + Simulation::getInstance()->infrastructure.drawing_origin;
        collision_point_front_position = gpos + forward * (car_length / 2.f + 30.f + 20.f * (speed - 1));
        return;
    }

    // refresh global bounding box and collision points from current local position
    {
        sf::Vector2f gpos = position + Simulation::getInstance()->infrastructure.drawing_origin;
        sf::RectangleShape temp({car_length, car_width});
        temp.setOrigin({car_length / 2.f, car_width / 2.f});
        temp.setPosition(gpos);
        temp.setRotation(moving_angle);
        boundingBox = temp.getGlobalBounds();
        sf::Vector2f fwd = {std::cos(moving_angle.asRadians()), std::sin(moving_angle.asRadians())};
        collision_point_front_position = gpos + fwd * (car_length / 2.f + 30.f + car_width * (speed - 1));
        collision_point_mask_position  = gpos + fwd * (car_length / 2.f + 10.f);
    }

    Intersection &next_intersection = Simulation::getInstance()->infrastructure.intersections[next_intersection_id];
    Intersection &current_intersection = Simulation::getInstance()->infrastructure.intersections[current_intersection_id];

    // enforce speed limits
    float speed_limit = max_speed;
    if (next_intersection.boundingBox.contains(collision_point_front_position) || (bool)boundingBox.findIntersection(next_intersection.boundingBox))
    {
        if (turning_direction == 0 || turning_direction == 2)
            speed_limit = 0.8f;
        else if (turning_direction == 3)
            speed_limit = 0.6f;
    }

    if (PointsCollidingWithRedLight(next_intersection))
        speed_limit = 0;

    if (PointsCollidingWithCar())
        speed_limit = 0;

    if (speed < speed_limit)
        speed += accerleration;
    else if (speed > speed_limit)
        speed -= breaking_force;
    if (speed < 0.f)
        speed = 0.f;

    for (int i = 0; i < (int)Simulation::getInstance()->traffic.vehicles.size(); i++)
    {
        if (Simulation::getInstance()->traffic.vehicles[i].boundingBox.contains(collision_point_mask_position))
            speed = 0;
    }

    sf::Vector2f forward = {std::cos(moving_angle.asRadians()), std::sin(moving_angle.asRadians())};

    bool on_intersection = (bool)boundingBox.findIntersection(next_intersection.boundingBox);
    if (on_intersection || (is_turning && turning_direction != 1 && turning_direction != -1))
    {
        if (!is_turning)
        {
            is_turning = true;
            turn_t = 0.f;
            entry_angle = moving_angle;
            entry_point = position; // arc the car center

            float road_t = Simulation::getInstance()->infrastructure.road_thickness;
            if (turning_direction == 0)       turning_radius = 3.f * road_t / 4.f + car_length / 2.f; // left
            else if (turning_direction == 2)  turning_radius = road_t / 4.f + car_length / 2.f;        // right
            else                              turning_radius = road_t / 4.f;                            // U-turn
        }

        if (turning_direction == 1 || turning_direction == -1)
        {
            // straight: just drive forward, no arc
            position.x += std::cos(entry_angle.asRadians()) * speed;
            position.y += std::sin(entry_angle.asRadians()) * speed;
            moving_angle = entry_angle;
        }
        else
        {
            float max_angle = (turning_direction == 3) ? 3.14159265f : (3.14159265f / 2.f);
            sf::Vector2f fwd = {std::cos(entry_angle.asRadians()), std::sin(entry_angle.asRadians())};
            sf::Vector2f side;
            if (turning_direction == 2)
                side = {-fwd.y, fwd.x};  // right
            else
                side = {fwd.y, -fwd.x};  // left / U-turn

            float arc_length = turning_radius * max_angle;
            turn_t += speed / arc_length;
            if (turn_t > 1.f) turn_t = 1.f;

            float theta = turn_t * max_angle;
            position = entry_point
                + fwd  * (turning_radius * std::sin(theta))
                + side * (turning_radius * (1.f - std::cos(theta)));

            sf::Vector2f dir = fwd * std::cos(theta) + side * std::sin(theta);
            moving_angle = dir.angle();
        }

        if (turn_t >= 1.f && turning_direction != 1 && turning_direction != -1)
        {
            is_turning = false;
            AdvanceToNextIntersection();
        }
    }
    else
    {
        if (is_turning && (turning_direction == 1 || turning_direction == -1))
        {
            is_turning = false;
            AdvanceToNextIntersection();
        }

        position.x += std::cos(moving_angle.asRadians()) * speed;
        position.y += std::sin(moving_angle.asRadians()) * speed;
    }

    forward = {std::cos(moving_angle.asRadians()), std::sin(moving_angle.asRadians())};
    sf::Vector2f gpos = position + Simulation::getInstance()->infrastructure.drawing_origin;
    collision_point_front_position = gpos + forward * (car_length / 2.f + 30.f + car_width * (speed - 1));
    collision_point_mask_position = gpos + forward * (car_length / 2.f + 10);
}

bool Vehicle::PointsCollidingWithCar(){
    for(int i = 0; i < (int)Simulation::getInstance()->traffic.vehicles.size(); i++){
        if(Simulation::getInstance()->traffic.vehicles[i].boundingBox.contains(collision_point_front_position))
        {
            return true;
        }
    }
    return false;
}


bool Vehicle::PointsCollidingWithRedLight(Intersection next_intersection){
    if (!(bool)boundingBox.findIntersection(next_intersection.boundingBox))
    {
        for (int i = 0; i < 4; i++)
        {
            if (next_intersection.light_boxes[i].contains(collision_point_front_position) && !(next_intersection.current_green_light_direction / 2 == i && next_intersection.current_green_light_direction % 2 == 1))
            {
                return true;
            }
        }
    }
    return false;
}

void Vehicle::CheckRemoveClick()
{
    if (!is_spawned || !sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        return;

    if (boundingBox.contains(GuiManager::getInstance()->mouse_position))
        deleted = true;
}

void Vehicle::Draw()
{
    if (!is_spawned)
        return;

    sf::Vector2f global_pos = position + Simulation::getInstance()->infrastructure.drawing_origin;

    // draw the vehicle
    sf::RectangleShape vehicle_shape({car_length, car_width});
    vehicle_shape.setOrigin({car_length / 2.f, car_width / 2.f});
    vehicle_shape.setPosition(global_pos);
    vehicle_shape.setRotation(moving_angle);
    GuiManager::getInstance()->window.draw(vehicle_shape);

    boundingBox = vehicle_shape.getGlobalBounds();

    if(GuiManager::getInstance()->draw_debug){
        // draw bounding box
        sf::RectangleShape boundingRect;
        boundingRect.setPosition(boundingBox.position);
        boundingRect.setSize(boundingBox.size);
        boundingRect.setFillColor(sf::Color::Transparent);
        boundingRect.setOutlineColor(sf::Color::Cyan);
        boundingRect.setOutlineThickness(1.f);
        GuiManager::getInstance()->window.draw(boundingRect);

        GuiManager::getInstance()->DrawDebugText(to_string(turning_direction), global_pos);

        // draw collision points
        sf::CircleShape collision_point_shape;
        collision_point_shape.setPosition({collision_point_front_position.x - 4.f, collision_point_front_position.y - 4.f});
        collision_point_shape.setRadius(4.f);
        collision_point_shape.setPointCount(8);
        collision_point_shape.setFillColor(sf::Color::Red);
        GuiManager::getInstance()->window.draw(collision_point_shape);

        collision_point_shape.setPosition({collision_point_mask_position.x - 4.f, collision_point_mask_position.y - 4.f});
        collision_point_shape.setRadius(4.f);
        collision_point_shape.setPointCount(8);
        collision_point_shape.setFillColor(sf::Color::Red);
        GuiManager::getInstance()->window.draw(collision_point_shape);
    }
}
