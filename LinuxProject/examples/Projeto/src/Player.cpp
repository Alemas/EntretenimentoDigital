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

//    SoundBuffer* knifeMeleeBuffer = new SoundBuffer();
//
//    SoundBuffer* handgunReloadBuffer = new SoundBuffer();
//    SoundBuffer* rifleReloadBuffer = new SoundBuffer();
//    SoundBuffer* shotgunReloadBuffer = new SoundBuffer();
//
//    SoundBuffer* handgunShootBuffer = new SoundBuffer();
//    SoundBuffer* rifleShootBuffer = new SoundBuffer();
//    SoundBuffer* shotgunShootBuffer = new SoundBuffer();
//
//    knifeMeleeBuffer->loadFromFile("data/soundeffects/knife_swing.wav");
//
//    handgunReloadBuffer->loadFromFile("data/soundeffects/handgun_reload.wav");
//    rifleReloadBuffer->loadFromFile("data/soundeffects/rifle_reload.wav");
//    shotgunReloadBuffer->loadFromFile("data/soundeffects/shotgun_reload.wav");
//
//    handgunShootBuffer->loadFromFile("data/soundeffects/handgun_shot.wav");
//    rifleShootBuffer->loadFromFile("data/soundeffects/rifle_shot.wav");
//    shotgunShootBuffer->loadFromFile("data/soundeffects/shotgun_shot.wav");


    weapons[0] = Weapon{Knife,
                        true, true, false,
                        0, 0, 0, 0, 0, 10,
                        zero, zero, zero,
                        zero, zero, seconds(0.2)};

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
                        70, 50, 5, 5, 10, 7,
                        zero, zero, zero,
                        seconds(0.6), coolDown, coolDown};

    feetSprite.load("data/img/survivor/survivor_feet_complete.png", 102, 102, 0, 0, 0, 0, 5, 17, 81);
    feetSprite.loadAnimation("data/img/survivor/survivor_feet_complete_anim.xml");
    feetSprite.setAnimRate(30);
    feetSprite.setOrigin(Vector2f(feetSprite.getSize().x/2, feetSprite.getSize().y/2));

    updateState(Top_Idle);
    updateFeetState(VECTOR_ZERO, false);

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

vector<Bullet*> Player::shoot() {

    Weapon w = weapons[currentWeapon];
    vector<Bullet*> r = vector<Bullet*>();

    if (!w.shoot || w.currentMagazine == 0) {
        return r;
    }

    Time elapsedTime = clock.getElapsedTime() - w.lastShootTime;

    if (elapsedTime >= w.shootCooldown) {

        reloading = false;

        weapons[currentWeapon].lastShootTime = clock.getElapsedTime();

        float rotation = Calculator::toRadians(topSprite.getRotation());
        Vector2f nonRotatedPosition = Vector2f(1.5, 1);

        if (w.type == Rifle || w.type == Shotgun) {
            nonRotatedPosition.x = 2.5;
            nonRotatedPosition.y = 0.8;
        }

        Vector2f position = Calculator::rotatedPoint(rotation, nonRotatedPosition);
        position.x = position.x + topSprite.getPosition().x/30;
        position.y = position.y + topSprite.getPosition().y/30;

        updateState(Shoot);

        weapons[currentWeapon].currentMagazine -= 1;

        if (w.type != Shotgun) {
            Vector2f direction = Vector2f(cos(rotation), sin(rotation));
            r.push_back(new Bullet(w.shootDamage, position, direction, w.type));
        } else {
            for (int i = -3; i < 4; i++) {
                float variant = Calculator::toRadians(10*i);
                Vector2f direction = Vector2f(cos(rotation + variant), sin(rotation + variant));

                Vector2f position = Calculator::rotatedPoint(rotation + variant, nonRotatedPosition);
                position.x = position.x + topSprite.getPosition().x/30;
                position.y = position.y + topSprite.getPosition().y/30;

                r.push_back(new Bullet(w.shootDamage, position, direction, w.type));
            }
        }
    }

    return r;
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
            //Necessário, senão o jogador pode ficar trocando de armas super rápido e atirar sempre sem ter que esperar o cooldown
            weapons[currentWeapon].lastShootTime = clock.getElapsedTime();
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

    //Gambiarra para alinhar as pernas quando está usando rifle ou shotgun

    float adjust = 1;
    if (weapons[currentWeapon].type == Rifle || weapons[currentWeapon].type == Shotgun) adjust = 0.8;

    Vector2f position = getPosition();
    Vector2f rotatedOrigin = Calculator::rotatedPoint(Calculator::toRadians(topSprite.getRotation()), Vector2f(topSprite.getOrigin().x - feetSprite.getSize().x*adjust,
                                                                                        topSprite.getOrigin().y - feetSprite.getSize().y + 10));
    float x = position.x + rotatedOrigin.x;
    float y = position.y + rotatedOrigin.y;

    feetSprite.setPosition(Vector2f(x, y));
    feetSprite.setRotation(topSprite.getRotation());

    screen->draw(feetSprite);
    screen->draw(topSprite);

    //screen->draw(lookingLine, 2, Lines);
}

void Player::update(float updateTimeInterval) {
    topSprite.update(updateTimeInterval, false);
    feetSprite.update(updateTimeInterval, false);

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
    updateFeetState(moveDirection, sprint);
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
            topSprite.setAnimRate(30);
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

void Player::printFeetState(FeetState state) {
    switch(state) {
    case Feet_Idle:
        cout << "Idle" << endl;
        break;
    case Walk:
        cout << "Walk" << endl;
        break;
    case Run:
        cout << "Run" << endl;
        break;
    case Strafe_Left:
        cout << "Strafe Left" << endl;
        break;
    case Strafe_Right:
        cout << "Strafe Right" << endl;
        break;
    }
}

void Player::updateFeetState(Vector2i direction, bool sprint) {

    float angle = 0;
    float x = direction.x;
    float y = direction.y;

    if (x == 0) {
        if (y == -1) angle = 270;
        if (y == 1) angle = 90;
    }
    if (x == 1) {
        if (y == -1) angle = 315;
        if (y == 1) angle = 45;
    }
    if (x == -1) {
        if (y == -1) angle = 225;
        if (y == 1) angle = 135;
        if (y == 0) angle = 180;
    }

    float difference = -(topSprite.getRotation() - 360) - angle;

    if (difference < 0) difference += 360;
    if (difference > 360) difference -= 360;

    FeetState newState;

    if (difference <= 45 || difference > 315) newState = Walk;
    if (difference > 45 && difference <= 135) newState = Strafe_Left;
    if (difference > 135 && difference <= 225) newState = Walk;
    if (difference > 225 && difference <= 315) newState = Strafe_Right;
    if (direction == VECTOR_ZERO) newState = Feet_Idle;

    if (newState == Walk && sprint) newState = Run;

//    printFeetState(newState);

    if (feetState != newState) {
        switch(newState) {
        case Feet_Idle:
            feetSprite.setAnimation("idle");
            break;
        case Walk:
            feetSprite.setAnimation("walk");
            break;
        case Run:
            feetSprite.setAnimation("run");
            break;
        case Strafe_Left:
            feetSprite.setAnimation("strafe_left");
            break;
        case Strafe_Right:
            feetSprite.setAnimation("strafe_right");
            break;
        }
        feetState = newState;
        feetSprite.play();
    }

}


