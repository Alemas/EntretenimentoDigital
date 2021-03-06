#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include<iostream>

#include <GameState.h>
#include <InputManager.h>
#include <Player.h>
#include <Enemy.h>
#include <Bullet.h>
#include <HUD.h>
#include <tmx/MapLoader.h>
#include <SFML/Graphics.hpp>

class GameplayState : public cgf::GameState
{
    public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);

    static GameplayState* instance() {
        return &m_GameplayState;
    }

    enum BodyID { PlayerID, EnemyID, WallID, BulletID, ItemID };

    protected:

    GameplayState() {}

    private:

    struct Wave {
        int number;
        bool isRunning;
        int totalEnemiesToKill, killedEnemies;
    };

    Wave wave = Wave{1, false, 10, 0};

    static GameplayState m_GameplayState;

    sf::RenderWindow* screen;
    cgf::InputManager* im;
    tmx::MapLoader* map;

    cgf::Physics* physics;
    bool firstTime;

    HUD hud = HUD();
    Player player;
    Enemy enemy;
    void centerMapOnPlayer();
    void updatePhysics();
    bool checkCollision(uint8_t layer, cgf::Game* game, cgf::Sprite* obj);
    sf::Uint16 getCellFromMap(uint8_t layernum, float x, float y);
    void startWave();
    void endWave();

};

#endif // GAMEPLAYSTATE_H
