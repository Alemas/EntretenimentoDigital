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
    Time coolDown = seconds(0.4);

    weapons[0] = Weapon{Knife,
                        true, true, false,
                        0, 0, 0, 0, 0, 10,
                        zero, zero, zero,
                        zero, seconds(0.5), seconds(0.2)};

    weapons[1] = Weapon{Pistol,
                        true, true, true,
                        100, 80, 20, 20, 10, 5,
                        zero, zero, zero,
                        seconds(0.4), coolDown, coolDown};

    weapons[2] = Weapon{Rifle,
                        true, true, true,
                        300, 270, 30, 30, 10, 7,
                        zero, zero, zero,
                        seconds(0.075), coolDown, coolDown};

    weapons[3] = Weapon{Shotgun,
                        true, true, true,
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

    Time elapsedTime = clock.getElapsedTime() - w.lastShootTime;

    if (elapsedTime >= w.shootCooldown) {

        reloading = false;

        weapons[currentWeapon].lastShootTime = clock.getElapsedTime();
        weapons[currentWeapon].currentMagazine -= 1;

        float rotation = Calculator::toRadians(topSprite.getRotation());

        Vector2f direction = Vector2f(cos(rotation), sin(rotation));

        Vector2f nonRotatedPosition = Vector2f(1.5, 1);

        if (w.type == Rifle || w.type == Shotgun) {
            nonRotatedPosition.x = 2.5;
            nonRotatedPosition.y = 0.8;
        }

        Vector2f position = Calculator::rotatedPoint(rotation, nonRotatedPosition);
        position.x = position.x + topSprite.getPosition().x/30;
        position.y = position.y + topSprite.getPosition().y/30;

        updateState(Shoot);

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
    updateState(Reload);
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

void Player::takeDamage(int damage) {

    if (isInvincible)
        return;

    if (damage >= health) {
        health = 0;
    } else {
        health -= damage;
    }

    topSprite.setColor(sf::Color::Red);
    isInvincible = true;
    lastDamageTookTime = clock.getElapsedTime();
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

            if (w.currentAmmo < w.magazineCapacity) {
                weapons[currentWeapon].currentMagazine = w.currentAmmo;
                weapons[currentWeapon].currentAmmo = 0;
            } else {
                weapons[currentWeapon].currentMagazine = w.magazineCapacity;
                weapons[currentWeapon].currentAmmo -= w.magazineCapacity;
            }

        }
    }

    if (isInvincible) {
        Time elapsedTime = clock.getElapsedTime() - lastDamageTookTime;
        if (elapsedTime > seconds(0.2)) {
            topSprite.setColor(Color::White);
        }
        if (elapsedTime >= invincibleCooldown) {
            isInvincible = false;
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
            topSprite.setOrigin(topSprite.getSize().x*0.5, topSprite.getSize().y/2.0);
    //        topSprite.setColor(Color::Blue);
            break;
        case 1:
            topSprite.load("data/img/survivor/survivor_handgun_complete.png", 179, 179, 0, 0, 0, 0, 10, 6, 58);
            topSprite.loadAnimation("data/img/survivor/survivor_handgun_complete_anim.xml");
            topSprite.setOrigin(topSprite.getSize().x*0.5, topSprite.getSize().y/2.0);
    //        topSprite.setColor(Color::Yellow);
            break;
        case 2:
            topSprite.load("data/img/survivor/survivor_rifle_complete.png", 179, 179, 0, 0, 0, 0, 11, 6, 63);
            topSprite.loadAnimation("data/img/survivor/survivor_rifle_complete_anim.xml");
            topSprite.setOrigin(topSprite.getSize().x*0.4, topSprite.getSize().y/2.0);
            break;
        case 3:
            topSprite.load("data/img/survivor/survivor_shotgun_complete.png", 179, 179, 0, 0, 0, 0, 11, 6, 63);
            topSprite.loadAnimation("data/img/survivor/survivor_shotgun_complete_anim.xml");
            topSprite.setOrigin(topSprite.getSize().x*0.4, topSprite.getSize().y/2.0);
            break;
        }
        //Necessário para atualizar a animação atual
        topState = Invalid;
    }

    if ((topState == Shoot || topState == Reload || topState == Melee) && topState != Invalid) {
        if (topSprite.isPlaying()) {
            return;
        }
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

         case Melee :
            if (weapons[currentWeapon].melee) {
                topSprite.setAnimRate(30);
                topSprite.setAnimation("meleeattack");
            } else {
                topState = Invalid;
            }
            break;

        case Shoot :
            if (weapons[currentWeapon].shoot) {
                topSprite.setAnimRate(30);
                topSprite.setAnimation("shoot");
            } else {
                topState = Invalid;
            }
            break;

        case Reload :
            if (weapons[currentWeapon].shoot) {
                topSprite.setAnimRate(30);
                topSprite.setAnimation("reload");
            } else {
                topState = Invalid;
            }
            break;

        default: ;
        }

        topState = state;
        topSprite.play();
    }
}

