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
        void shoot();
        void reload();
        void changeWeapon(int slot);

    protected:

    private:
    enum FeetState {Feet_Idle, Walk, Run, Strafe_Left, Strafe_Right};
    enum TopState {Top_Idle, Top_Walk, Top_Run, Melee, Shoot, Reload};

    FeetState feetState;
    TopState topState = Reload;

    int walkSpeed = 150;
    int runSpeed = 220;

    cgf::Sprite feetSprite;

    void updateState(TopState state);
};

#endif // PLAYER_H
