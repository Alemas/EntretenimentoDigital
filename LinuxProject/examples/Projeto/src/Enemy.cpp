#include "Enemy.h"

#define VECTOR_ZERO Vector2f (0,0)

using namespace sf;
using namespace std;

Enemy::Enemy(){

}

Enemy::~Enemy(){

}

void Enemy::init(sf::Vector2i position){

    load("data/img/survivor/survivor1.png", 179, 179, 0, 0, 0, 0, 5, 8);
    loadAnimation("data/img/survivor/survivor1_anim.xml");
    setColor(Color(255,0,0,255));
    setOrigin(getSize().x/2.0, getSize().y/2.0);
    setPosition(Vector2f(position));
    updateState(Top_Idle);
    play();
}

void Enemy::movement(Player player){

    bool shouldUpdateAnimation = false;

    bool sprint = false;

    float topRotation = Calculator::angleBetweenPoints(getPosition(), Vector2f(player.getPosition().x, player.getPosition().y));
    topRotation = Calculator::toDegrees(topRotation);

    setRotation(topRotation);

    TopState s = Top_Idle;

    if (getPosition() == VECTOR_ZERO) {
        setXspeed(0);
        setYspeed(0);
        s = Top_Idle;
    }
     else {
        if (sprint) {
            setXspeed(getPosition().x * runSpeed);
            setYspeed(getPosition().y * runSpeed);
            s = Top_Run;
        } else {
            setXspeed(player.getPosition().x - getPosition().x * walkSpeed);
            setYspeed(player.getPosition().y - getPosition().y * walkSpeed);
            s = Top_Walk;
        }
    }

    updateState(s);

}


void Enemy::updateState(TopState state) {

    if (state != topState) {
        switch(state) {
        case Top_Idle :
            setAnimRate(15);
            setAnimation("idle");
            break;

        case Top_Walk :
            setAnimRate(30);
            setAnimation("move");
            break;

        case Top_Run :
            setAnimRate(55);
            setAnimation("move");
            break;

        default: ;
        }

        topState = state;
        play();
    }
}
