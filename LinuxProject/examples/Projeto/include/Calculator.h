#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <SFML/Graphics.hpp>
#include<cmath>
#define M_PI 3.14159265358979323846

class Calculator
{
    public:

        static float angleBetweenPoints(sf::Vector2f p1, sf::Vector2f p2);
        static float toDegrees(float radiansValue);
        static sf::Vector2f rotatedPoint(float rotation, sf::Vector2f point);

    protected:

        Calculator() {}

    private:

};

#endif // CALCULATOR_H
