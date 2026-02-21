#include <SFML/Graphics.hpp>

using namespace std;

class Intersection {
    public:

    int id;
    sf::Vector2f position;
    bool has_traffic_lights;

    Intersection(int _id, sf::Vector2f _position, bool _has_traffic_lights){
        id = _id;
        position = _position;
        has_traffic_lights = _has_traffic_lights;
    }
};
