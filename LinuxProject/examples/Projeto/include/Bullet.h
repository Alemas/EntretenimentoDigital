#ifndef BULLET_H
#define BULLET_H

#include <Sprite.h>
#include <iostream>
#include "Calculator.h"
#include "Physics.h"
#include <SFML/Graphics.hpp>

class Bullet
{
    public:
        Bullet(int damage, sf::Vector2f position, sf::Vector2f direction);
        virtual ~Bullet();
        int getDamage() {return damage;}
        b2Body* body;
        cgf::Sprite sprite;

    protected:

    private:
        int damage;
        cgf::Physics* physics;
};

#endif // BULLET_H
