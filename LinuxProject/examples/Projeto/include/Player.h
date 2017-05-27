#ifndef PLAYER_H
#define PLAYER_H

#include <Sprite.h>
#include <iostream>
#include "Calculator.h"
#include "Physics.h"
#include <SFML/Graphics.hpp>

class Player
{
    public:
        Player();
        virtual ~Player();
        b2Body* body;

        void init();

        void updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint);
        void shoot();
        void reload();
        void changeWeapon(int slot);
        void draw(sf::RenderWindow* screen);
        void update(float deltaTime);
        sf::Vector2f getPosition() { return topSprite.getPosition(); }

    protected:

    private:
    enum FeetState {Feet_Idle, Walk, Run, Strafe_Left, Strafe_Right};
    enum TopState {Top_Idle, Top_Walk, Top_Run, Melee, Shoot, Reload};

    cgf::Physics* physics;

    FeetState feetState;
    TopState topState = Reload;

    int walkSpeed = 2000;
    int runSpeed = 4000;

    cgf::Sprite topSprite;
    cgf::Sprite feetSprite;

    void updateState(TopState state);
};

#endif // PLAYER_H
