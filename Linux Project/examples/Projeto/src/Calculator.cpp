#include "Calculator.h"

float Calculator::angleBetweenPoints(sf::Vector2f p1, sf::Vector2f p2) {

    float len1 = sqrt(p1.x * p1.x + p1.y * p1.y);
    float len2 = sqrt(p2.x * p2.x + p2.y * p2.y);

    float dot = p1.x * p2.x + p1.y * p2.y;

    float a = dot / (len1 * len2);

    if (a >= 1.0)
        return 0.0;
    else if (a <= -1.0)
        return M_PI;
    else
        return acos(a);
}

float Calculator::toDegrees(float radiansValue) {
    return (180.0/M_PI) * radiansValue;
}
