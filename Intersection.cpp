#include <SFML/Graphics.hpp>
#include "Intersection.h"
#include "GuiManager.h"

using namespace std;

Intersection::Intersection(int _id, sf::Vector2f _position, bool _has_traffic_lights){
    id = _id;
    position = _position;
    has_traffic_lights = _has_traffic_lights;
    used = false;
    boundingBox.position = {position.x - intersection_size / 2, position.y - intersection_size / 2};
    boundingBox.size = {intersection_size, intersection_size};
}

void Intersection::Update(){

}

void Intersection::Draw(){
// draw intersections
    sf::RectangleShape intersection_shape({intersection_size, intersection_size});
    intersection_shape.setPosition({position.x - intersection_size / 2, position.y - intersection_size / 2});

    if(used){
        intersection_shape.setFillColor(sf::Color(90, 90, 90));
    }else{
        intersection_shape.setFillColor(sf::Color(20, 20, 20));
    }

    GuiManager::getInstance()->window.draw(intersection_shape);

    GuiManager::getInstance()->DrawText(to_string(id), position);

    //sf::FloatRect boundingBox = intersection_shape.getGlobalBounds();
    sf::RectangleShape boundingRect;
    boundingRect.setPosition(boundingBox.position);
    boundingRect.setSize(boundingBox.size);
    boundingRect.setFillColor(sf::Color::Transparent);
    boundingRect.setOutlineColor(sf::Color::Cyan);
    boundingRect.setOutlineThickness(1.0f);
    GuiManager::getInstance()->window.draw(boundingRect);
}
