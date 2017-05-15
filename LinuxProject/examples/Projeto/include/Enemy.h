#ifndef ENEMY_H
#define ENEMY_H

#include <Sprite.h>
#include <iostream>
#include <Player.h>
#include "Calculator.h"

class Enemy:public cgf::Sprite{

    public:
        Enemy();
        virtual ~Enemy();

        void init(sf::Vector2i position);

        void movement(Player player);

        protected:

    private:
    enum FeetState {Feet_Idle, Walk, Run, Strafe_Left, Strafe_Right};
    enum TopState {Top_Idle, Top_Walk, Top_Run, Melee, Shoot, Reload};

    FeetState feetState;
    TopState topState = Reload;

    int walkSpeed = 1;
    int runSpeed = 150;

    cgf::Sprite feetSprite;

    void updateState(TopState state);

};
#endif // ENEMY_H
