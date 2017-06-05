#include "Player.h"
#include "GameplayState.h"

#define VECTOR_ZERO Vector2i(0,0)

using namespace sf;
using namespace std;
using namespace cgf;

Player::Player()
{

}

Player::~Player()
{
    //dtor
}

void Player::init() {

    clock = Clock();

    weapons[0] = Weapon{Knife,
                        true, true, false,
                        0, 0, 0, 0, 10,
                        0, 0, 0,
                        0, 0, 0.1};

    weapons[1] = Weapon{Pistol,
                        true, true, true,
                        0, 0, 0, 0, 10,
                        0, 0, 0,
                        0, 0, 0.1};

    weapons[2] = Weapon{Shotgun,
                        false, true, true,
                        0, 0, 0, 0, 10,
                        0, 0, 0,
                        0, 0, 0.1};

    weapons[3] = Weapon{Rifle,
                        false, true, true,
                        0, 0, 0, 0, 10,
                        0, 0, 0,
                        0, 0, 0.1};

    topSprite.load("data/img/survivor/survivor1.png", 179, 179, 0, 0, 0, 0, 5, 8);
    topSprite.loadAnimation("data/img/survivor/survivor1_anim.xml");

//    load("data/img/survivor/survivor_handgun_move.png", 258, 220, 0, 0, 0, 0, 7, 3, 20);
//    loadAnimation("data/img/survivor/survivor_handgun_move_anim.xml");
//
//    load("data/img/survivor/survivor_handgun_move.png", 258, 220, 0, 0, 0, 0, 7, 3, 20);
//    loadAnimation("data/img/survivor/survivor_handgun_idle_anim.xml");

//    setScale(0.5, 0.5);
    topSprite.setOrigin(topSprite.getSize().x/2.0, topSprite.getSize().y/2.0);

    updateState(Top_Idle);

    physics = cgf::Physics::instance();

    body = physics->newCircle(GameplayState::BodyID::PlayerID, 0, 0, 12.5, 400, 0.5, 1.0, false);
    physics->setImage(body, &topSprite);
    body->SetFixedRotation(false);
    body->SetLinearDamping(10);
}

void Player::meleeAttack() {
//    if (weapons[currentWeapon].melee) {
//        if (weapons[currentWeapon].lastMeleeTime) {
//        }
//    }
}

void Player::shoot() {
    //usa a rotation do proprio sprite
}

void Player::reload() {
    //timer que impede de atirar, tbm torna reloading true
}

void Player::changeWeapon(int slot) {
    if (slot >= 0 && slot <= 3) {
        if (weapons[slot].unlocked) {
            if (currentWeapon != slot)
                currentWeapon = slot;
        }
    }
}

void Player::draw(RenderWindow* screen) {
    screen->draw(topSprite);
    screen->draw(lookingLine, 2, Lines);
}

void Player::update(float updateTimeInterval) {
    topSprite.update(updateTimeInterval, false);
}

void Player::updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint) {

    b2Vec2 bodyPos = physics->getPosition(body);
    Vector2f pos = Vector2f(bodyPos.x, bodyPos.y);

    lookingLine[0].position = pos;
    lookingLine[0].color = Color::Yellow;
    lookingLine[1].position = Vector2f(lookingPoint.x, lookingPoint.y);
    lookingLine[1].color = Color::Yellow;

    float topRotation = Calculator::angleBetweenPoints(pos, Vector2f(lookingPoint.x, lookingPoint.y));
    body->SetTransform(body->GetPosition(), topRotation);

//    topRotation = Calculator::toDegrees(topRotation);
//    topSprite.setRotation(topRotation);

    TopState s = Top_Idle;

    if (moveDirection == VECTOR_ZERO) {
        s = Top_Idle;
    } else {
        if (sprint) {
            body->ApplyLinearImpulse(b2Vec2(moveDirection.x*runSpeed, moveDirection.y*runSpeed), body->GetWorldCenter(), true);
            s = Top_Run;
        } else {
            body->ApplyLinearImpulse(b2Vec2(moveDirection.x*walkSpeed, moveDirection.y*walkSpeed), body->GetWorldCenter(), true);
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
            topSprite.setAnimRate(15);
            topSprite.setAnimation("idle");
            break;

        case Top_Walk :
            topSprite.setAnimRate(30);
            topSprite.setAnimation("move");
            break;

        case Top_Run :
            topSprite.setAnimRate(55);
            topSprite.setAnimation("move");
            break;

        default: ;
        }

        topState = state;
        topSprite.play();
    }
}

