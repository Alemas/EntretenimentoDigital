#include "Calculator.h"

float Calculator::angleBetweenPoints(sf::Vector2f p1, sf::Vector2f p2) {

    float deltaY = p2.y - p1.y;
    float deltaX = p2.x - p1.x;

    return atan2(deltaY, deltaX);

}

float Calculator::toDegrees(float radiansValue) {
    return (180.0/M_PI) * radiansValue;
}
