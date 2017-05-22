#include "Player.h"
#include "GameplayState.h"

#define VECTOR_ZERO Vector2i(0,0)

using namespace sf;
using namespace std;

Player::Player()
{

}

Player::~Player()
{
    //dtor
}

void Player::init() {

    load("data/img/survivor/survivor1.png", 179, 179, 0, 0, 0, 0, 5, 8);
    loadAnimation("data/img/survivor/survivor1_anim.xml");

//    load("data/img/survivor/survivor_handgun_move.png", 258, 220, 0, 0, 0, 0, 7, 3, 20);
//    loadAnimation("data/img/survivor/survivor_handgun_move_anim.xml");
//
//    load("data/img/survivor/survivor_handgun_move.png", 258, 220, 0, 0, 0, 0, 7, 3, 20);
//    loadAnimation("data/img/survivor/survivor_handgun_idle_anim.xml");

//    setScale(0.5, 0.5);
    setOrigin(getSize().x/2.0, getSize().y/2.0);
    setPosition(500, 500);

    updateState(Top_Idle);
    play();

    physics = cgf::Physics::instance();
    body = physics->newCircle(GameplayState::BodyID::PlayerID, getOrigin().x, getOrigin().y, 30, 50, 0.5, 1.0, false);
}

void Player::shoot() {
    //usa a rotation do proprio sprite
}

void Player::reload() {
    //timer que impede de atirar, tbm roda torna reloading true
}

void Player::changeWeapon(int slot) {

}

void Player::updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint) {

    bool shouldUpdateAnimation = false;

    float topRotation = Calculator::angleBetweenPoints(getPosition(), Vector2f(lookingPoint.x, lookingPoint.y));
    topRotation = Calculator::toDegrees(topRotation);

//    cout << topRotation << endl;

    setRotation(topRotation);

    TopState s = Top_Idle;

    if (moveDirection == VECTOR_ZERO) {
        setXspeed(0);
        setYspeed(0);
        s = Top_Idle;
    } else {
        if (sprint) {
            setXspeed(moveDirection.x * runSpeed);
            setYspeed(moveDirection.y * runSpeed);
            s = Top_Run;
        } else {
            setXspeed(moveDirection.x * walkSpeed);
            setYspeed(moveDirection.y * walkSpeed);
            s = Top_Walk;
        }
    }

    updateState(s);
//    cout << moveDirection.x << ", " << moveDirection.y << endl;
}

void Player::updateState(TopState state) {

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

