#ifndef SPLASHSTATE_H
#define SPLASHSTATE_H

#include <iostream>

#include "GameState.h"
#include "MenuState.h"

class SplashState : public cgf::GameState
{
    public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);

    static SplashState* instance() {
        return &m_SplashState;
    }

    protected:

    SplashState() {}

    private:

    static SplashState m_SplashState;

    sf::Clock infoTimer;
    float timeLimit = 0.7;

    sf::Font font;
    sf::Text title;
    sf::Text info;
    sf::RenderWindow* screen;


};

#endif // SPLASHSTATE_H
