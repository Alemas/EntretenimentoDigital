#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include <GameState.h>
#include <Player.h>

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

    protected:

    GameplayState() {}

    private:

    static GameplayState m_GameplayState;

    sf::RenderWindow* screen;
    Player player;

};

#endif // GAMEPLAYSTATE_H
