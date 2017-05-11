#include "Player.h"

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

    load("data/img/Char14.png");
    setPosition(500, 500);
    setScale(3.0, 3.0);

}

void Player::updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool run) {

    bool shouldUpdateAnimation = false;

    float topRotation = Calculator::angleBetweenPoints(getPosition(), Vector2f(lookingPoint.x, lookingPoint.y));
    topRotation = Calculator::toDegrees(topRotation);

    cout << topRotation << endl;

    setRotation(topRotation);

}
