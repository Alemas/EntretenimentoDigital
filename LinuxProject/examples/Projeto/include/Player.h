#ifndef PLAYER_H
#define PLAYER_H

#include <Sprite.h>
#include <iostream>
#include <Bullet.h>
#include "Calculator.h"
#include "Physics.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#define MAX_HEALTH 100

class Player
{
    public:

        enum WeaponType {Knife = 0, Pistol = 1, Rifle = 2, Shotgun = 3};

        struct Weapon {
            WeaponType type;
            bool unlocked;
            bool melee;
            bool shoot;
            int maxAmmo, currentAmmo, magazineCapacity, currentMagazine, shootDamage, meleeDamage;
            sf::Time lastShootTime, lastReloadTime, lastMeleeTime;
            sf::Time shootCooldown, reloadTime, meleeCooldown;
//            sf::SoundBuffer* reloadBuffer; sf::SoundBuffer* shootBuffer; sf::SoundBuffer* meleeBuffer;
        };

        Player();
        virtual ~Player();
        b2Body* body;

        void init();

        void updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint);
        void meleeAttack();
        std::vector<Bullet*> shoot();
        void reload();

          void draw(sf::RenderWindow* screen);
        void update(float deltaTime);
        sf::Vector2f getPosition() { return topSprite.getPosition(); }
        void setPosition(b2Vec2 pos);
        Weapon getCurrentWeapon() { return weapons[currentWeapon]; }
        void changeWeapon(int slot);

        int getHealth() { return health; }
        int getMaxHealth() { return MAX_HEALTH; }
        void takeDamage(int damage);
        bool isAlive() { return health > 0; }

    protected:

    private:
        enum FeetState {Feet_Idle, Walk, Run, Strafe_Left, Strafe_Right};
        enum TopState {Top_Idle, Top_Walk, Top_Run, Melee, Shoot, Reload, Invalid};

        Weapon weapons[4];
        int currentWeapon = 0;
        int lastWeapon = -1;

        int health = MAX_HEALTH;
        sf::Time invincibleCooldown = sf::seconds(0.5);
        sf::Time lastDamageTookTime = sf::seconds(0);
        bool isInvincible = false;

        bool reloading = false;
        sf::Time lastReloadTime;

        sf::Clock clock;

        cgf::Physics* physics;

        FeetState feetState = Feet_Idle;
        TopState topState = Invalid;

        int walkSpeed = 2000;
        int runSpeed = 3500;

        cgf::Sprite topSprite;
        cgf::Sprite feetSprite;

//        sf::Sound weaponSound;

        sf::Vertex lookingLine[2];

        void updateState(TopState state);
        void updateFeetState(sf::Vector2i direction, bool sprint);
        void printFeetState(FeetState state);
};

#endif // PLAYER_H
