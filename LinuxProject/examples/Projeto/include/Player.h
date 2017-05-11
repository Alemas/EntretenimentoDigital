#ifndef PLAYER_H
#define PLAYER_H

#include <Sprite.h>
#include <iostream>
#include "Calculator.h"


class Player : public cgf::Sprite
{
    public:
        Player();
        virtual ~Player();

        void init();

        void updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint);

    protected:

    private:
    enum FeetState {Feet_Idle, Walk, Run, Strafe_Left, Strafe_Right};
    enum TopState {Top_Idle, Move, Melee, Shoot, Reload};

    FeetState feetState = FeetState::Feet_Idle;
    TopState topState = TopState::Top_Idle;

    int walkSpeed = 100;
    int runSpeed = 200;

    cgf::Sprite feetSprite;

    void updateAnimation();

};

#endif // PLAYER_H
