#include "SplashState.h"

SplashState SplashState::m_SplashState;

using namespace std;
using namespace sf;

void SplashState::init() {

    if (!font.loadFromFile("data/fonts/Sansation_Regular.ttf")) {
        cout << "Cannot load Sansation_Regular.ttf font" << endl;
        exit(1);
    }

    title.setFont(font);
    info.setFont(font);

    title.setString("The Last Survivor");
    info.setString("Press ENTER to continue");

    title.setCharacterSize(150);
    info.setCharacterSize(50);

    title.setFillColor(Color::White);
    info.setFillColor(Color::White);

    title.setPosition(0.0, 0.0);
    info.setPosition(0.0, 0.0);

    cout << "SplashState: init" << endl;

}

void SplashState::cleanup() {
    cout << "SplashState: cleanup" << endl;
}

void SplashState::pause() {
    cout << "SplashState: pause" << endl;
}

void SplashState::resume() {
    cout << "SplashState: resume" << endl;
}

void SplashState::handleEvents(cgf::Game* game) {

    screen = game->getScreen();
    Event event;

    while (screen->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            game->quit();
    }

    if (Keyboard::isKeyPressed(Keyboard::Key::Return)) {
        changeState(game, MenuState::instance());
    }

    if (infoTimer.getElapsedTime().asSeconds() > timeLimit) {

        if (info.getFillColor() == Color::Transparent)
            info.setFillColor(Color::White);
        else
            info.setFillColor(Color::Transparent);

        infoTimer.restart();
    }

    cout << "SplashState: handleEvents" << endl;
}

void SplashState::update(cgf::Game* game) {

    screen = game->getScreen();

    if(title.getPosition().x == 0.0) { title.setPosition(screen->getSize().x/2 - title.getLocalBounds().width/2, screen->getSize().y/2 - 250); }
    if(info.getPosition().x == 0.0) { info.setPosition(screen->getSize().x/2 - info.getLocalBounds().width/2, screen->getSize().y/2); }

    cout << "SplashState: update" << endl;

}

void SplashState::draw(cgf::Game* game) {

    screen = game->getScreen();

    screen->draw(title);
    screen->draw(info);

    cout << "SplashState: draw" << endl;
}
