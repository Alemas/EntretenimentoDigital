#ifndef ENEMY_H
#define ENEMY_H

#include <Sprite.h>
#include <iostream>
#include <Bullet.h>
#include "Calculator.h"
#include "Physics.h"
#include <SFML/Graphics.hpp>

class Enemy
{
    public:
        Enemy();
        virtual ~Enemy();
        b2Body* body;

        void init();

        void updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint);
        void meleeAttack();
        void draw(sf::RenderWindow* screen);
        void update(float deltaTime);
        sf::Vector2f getPosition() { return topSprite.getPosition(); }
        void setPosition(b2Vec2 pos);

    protected:

    private:
    enum FeetState {Feet_Idle, Walk, Run, Strafe_Left, Strafe_Right};
    enum TopState {Top_Idle, Top_Walk, Top_Run, Melee, Shoot, Reload, Invalid};

    sf::Clock clock;

    cgf::Physics* physics;

    FeetState feetState;
    TopState topState = Reload;

    int walkSpeed = 2000;
    int runSpeed = 3500;

    cgf::Sprite topSprite;
    cgf::Sprite feetSprite;

    sf::Vertex lookingLine[2];

    void updateState(TopState state);
};

#endif // PLAYER_H
