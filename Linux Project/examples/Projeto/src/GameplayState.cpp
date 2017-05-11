#include "GameplayState.h"

using namespace std;
using namespace sf;

GameplayState GameplayState::m_GameplayState;

void GameplayState::init() {
    player.init();
}

void GameplayState::cleanup() {

}

void GameplayState::pause() {

}

void GameplayState::resume() {

}

void GameplayState::handleEvents(cgf::Game* game) {

    screen = game->getScreen();
    Event event;

    while(screen->pollEvent(event)) {
        switch (event.type) {
        case Event::EventType::MouseMoved :
            player.updateMovement(Vector2i(event.mouseMove.x, event.mouseMove.y), Vector2i(0,0), false);
            break;
        default : ;

        }
    }
}

void GameplayState::update(cgf::Game* game) {

}

void GameplayState::draw(cgf::Game* game) {
    screen = game->getScreen();

    screen->draw(player);
}

