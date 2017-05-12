#include "Player.h"

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

    load("data/img/survivor/survivor_handgun_move.png", 258, 220, 0, 0, 0, 0, 7, 3, 20);
    loadAnimation("data/img/survivor/survivor_handgun_move_anim.xml");

//    load("data/img/survivor/survivor_handgun_idle.png", 253, 216, 0, 0, 0, 0, 4, 5, 20);
//    loadAnimation("data/img/survivor/survivor_handgun_idle_anim.xml");

    setAnimation("move");
    setAnimRate(20);
    play();

    setOrigin(getSize().x/3, getSize().y - 80);
    setPosition(500, 500);
    setScale(0.7, 0.7);

}

void Player::updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint) {

    bool shouldUpdateAnimation = false;

    float topRotation = Calculator::angleBetweenPoints(getPosition(), Vector2f(lookingPoint.x, lookingPoint.y));
    topRotation = Calculator::toDegrees(topRotation);

//    cout << topRotation << endl;

    setRotation(topRotation);

    if (sprint) {
        setXspeed(moveDirection.x * runSpeed);
        setYspeed(moveDirection.y * runSpeed);
    } else {
        setXspeed(moveDirection.x * walkSpeed);
        setYspeed(moveDirection.y * walkSpeed);
    }

    if (moveDirection == VECTOR_ZERO)
        pause();
    else
        play();

//    cout << moveDirection.x << ", " << moveDirection.y << endl;
}
