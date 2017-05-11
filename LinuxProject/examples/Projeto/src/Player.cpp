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

    load("data/img/Top_Down_Survivor/handgun/idle/survivor-idle_handgun_0.png");
    setOrigin(getSize().x/2, getSize().y/2);
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

//    cout << moveDirection.x << ", " << moveDirection.y << endl;
}
