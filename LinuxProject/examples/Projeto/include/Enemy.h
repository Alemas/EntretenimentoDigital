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
    enum State {Idle, Walk, Run, Melee, Shoot, Reload};

    State state = Idle;

    int walkSpeed = 1;
    int runSpeed = 150;

    void updateState(State state);

};
#endif // ENEMY_H
