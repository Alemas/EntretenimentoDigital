#include "GameplayState.h"

using namespace std;
using namespace sf;
using namespace cgf;

GameplayState GameplayState::m_GameplayState;

void GameplayState::init() {
    player.init();
    im = InputManager::instance();

    im->addKeyInput("Left", Keyboard::A);
    im->addKeyInput("Right", Keyboard::D);
    im->addKeyInput("Up", Keyboard::W);
    im->addKeyInput("Down", Keyboard::S);
    im->addKeyInput("Num1", Keyboard::Num1);
    im->addKeyInput("Num2", Keyboard::Num2);
    im->addKeyInput("Num3", Keyboard::Num3);
    im->addKeyInput("Num4", Keyboard::Num4);

    im->addMouseInput("LeftClick", Mouse::Left);

    cout << "GameplayState: init" << endl;
}

void GameplayState::cleanup() {

    cout << "GameplayState: cleanup" << endl;
}

void GameplayState::pause() {

    cout << "GameplayState: pause" << endl;
}

void GameplayState::resume() {

    cout << "GameplayState: resume" << endl;
}

void GameplayState::handleEvents(cgf::Game* game) {

    screen = game->getScreen();
    Event event;

    while(screen->pollEvent(event)) {
        switch (event.type) {
        case Event::EventType::Closed :
            game->quit();
        default : ;

        }
    }

    int dirx = 0;
    int diry = 0;

    if (im->testEvent("Left")) {
        dirx -= 1;
    }
    if (im->testEvent("Right")) {
        dirx += 1;
    }
    if (im->testEvent("Up")) {
        diry -= 1;
    }
    if (im->testEvent("Down")) {
        diry += 1;
    }

    player.updateMovement(Mouse::getPosition(*screen), Vector2i(dirx,diry), false);

    cout << "GameplayState: handleEvents" << endl;
}

void GameplayState::update(cgf::Game* game) {

    screen = game->getScreen();

    player.update(game->getUpdateInterval(), true);

    cout << "GameplayState: update" << endl;
}

void GameplayState::draw(cgf::Game* game) {
    screen = game->getScreen();

    screen->draw(player);

    cout << "GameplayState: draw" << endl;
}

