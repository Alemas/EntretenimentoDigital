#include "Player.h"
#include "GameplayState.h"

#define VECTOR_ZERO Vector2i(0,0)
#define VECTOR_ZEROf Vector2f(0,0)

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

    Time zero = Time().Zero;
    Time coolDown = seconds(2.0);

    weapons[0] = Weapon{Knife,
                        true, true, false,
                        0, 0, 0, 0, 0, 10,
                        zero, zero, zero,
                        zero, zero, coolDown};

    weapons[1] = Weapon{Pistol,
                        true, true, true,
                        100, 80, 20, 20, 0, 10,
                        zero, zero, zero,
                        coolDown, coolDown, coolDown};

    weapons[2] = Weapon{Shotgun,
                        false, true, true,
                        0, 0, 0, 0, 0, 10,
                        zero, zero, zero,
                        coolDown, coolDown, coolDown};

    weapons[3] = Weapon{Rifle,
                        false, true, true,
                        0, 0, 0, 0, 0, 10,
                        zero, zero, zero,
                        coolDown, coolDown, coolDown};

    updateState(Top_Idle);

    physics = cgf::Physics::instance();

    body = physics->newCircle(GameplayState::BodyID::PlayerID, 0, 0, 12.5, 400, 0.5, 1.0, false);
    physics->setImage(body, &topSprite);
    body->SetFixedRotation(false);
    body->SetLinearDamping(10);

    lastReloadTime = zero;
}

void Player::setPosition(b2Vec2 pos) {
    physics->setPosition(body, pos);
}

void Player::meleeAttack() {
//    if (weapons[currentWeapon].melee) {
//        Weapon w = weapons[currentWeapon];
//        Time elapsedTime = clock.getElapsedTime() - w.lastMeleeTime;
//        if (elapsedTime >= w->meleeCooldown) {
//
//        }
//    }
}

Bullet* Player::shoot() {

    Weapon w = weapons[currentWeapon];

    if (!w.shoot || w.currentMagazine == 0) {
        return nullptr;
    }

    reloading = false;

    Time elapsedTime = clock.getElapsedTime() - w.lastShootTime;

    if (elapsedTime >= w.shootCooldown) {

        w.lastShootTime = clock.getElapsedTime();
        w.currentMagazine -= 1;

        Vector2f direction = Vector2f(cos(topSprite.getRotation()), sin(topSprite.getRotation()));

        Vector2f position = Calculator::rotatedPoint(topSprite.getRotation(), Vector2f(3.0, 0));
        position.x = position.x + topSprite.getPosition().x/30;
        position.y = position.y + topSprite.getPosition().y/30;

        return new Bullet(w.shootDamage, position, direction);
    }

    return nullptr;
}

void Player::reload() {
    Weapon w = weapons[currentWeapon];
    if (!w.shoot || reloading ||  w.currentAmmo == 0 || w.currentMagazine == w.magazineCapacity) {
        return;
    }
    reloading = true;
    lastReloadTime = clock.getElapsedTime();
}

void Player::changeWeapon(int slot) {
    if (slot >= 0 && slot <= 3) {
        if (weapons[slot].unlocked) {
            if (currentWeapon != slot) {
                reloading = false;
            }
            currentWeapon = slot;
        }
    }
}

void Player::draw(RenderWindow* screen) {
    screen->draw(topSprite);
    //screen->draw(lookingLine, 2, Lines);
}

void Player::update(float updateTimeInterval) {
    topSprite.update(updateTimeInterval, false);

    if (reloading) {
        Time elapsedTime = clock.getElapsedTime() - lastReloadTime;
        Weapon w = weapons[currentWeapon];

        if (elapsedTime >= w.reloadTime) {
            reloading = false;

            if (w.currentAmmo > w.magazineCapacity) {
                w.currentMagazine = w.currentAmmo;
                w.currentAmmo = 0;
            } else {
                w.currentMagazine = w.magazineCapacity;
                w.currentAmmo -= w.magazineCapacity;
            }

        }
    }
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

    if (lastWeapon != currentWeapon) {

        lastWeapon = currentWeapon;

        switch(currentWeapon) {
        case 0:
            topSprite.load("data/img/survivor/survivor_knife_complete.png", 179, 179, 0, 0, 0, 0, 11, 5);
            topSprite.loadAnimation("data/img/survivor/survivor_knife_complete_anim.xml");
    //        topSprite.setColor(Color::Blue);
            break;
        case 1:
            topSprite.load("data/img/survivor/survivor_handgun.png", 179, 179, 0, 0, 0, 0, 5, 8);
            topSprite.loadAnimation("data/img/survivor/survivor_handgun_anim.xml");
    //        topSprite.setColor(Color::Yellow);
            break;
        case 2:
            topSprite.setColor(Color::Red);
            break;
        case 3:
            topSprite.setColor(Color::Green);
            break;
        }

        topSprite.setOrigin(topSprite.getSize().x/2.0, topSprite.getSize().y/2.0);
        //Necessário para atualizar a animação atual
        topState = Invalid;
    }

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

