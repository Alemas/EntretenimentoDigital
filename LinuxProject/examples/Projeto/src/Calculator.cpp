#include "Calculator.h"

using namespace sf;

float Calculator::angleBetweenPoints(sf::Vector2f p1, sf::Vector2f p2) {

    float deltaY = p2.y - p1.y;
    float deltaX = p2.x - p1.x;

    return atan2(deltaY, deltaX);

}

float Calculator::toDegrees(float radiansValue) {
    return (180.0/M_PI) * radiansValue;
}

float Calculator::toRadians(float degreesValue) {
    return (M_PI/180.0) * degreesValue;
}

sf::Vector2f Calculator::rotatedPoint(float rotation, sf::Vector2f point) {

    float x = point.x;
    float y = point.y;

    Vector2f r = Vector2f();
    r.x = x * cos(rotation) - y * sin(rotation);
    r.y = x * sin(rotation) + y * cos(rotation);

    return r;

}

