#include "MenuState.h"

using namespace std;
using namespace sf;

MenuState MenuState::m_MenuState;

void MenuState::init() {

    if (!font.loadFromFile("data/fonts/Sansation_Regular.ttf")) {
        cout << "Cannot load Sansation_Regular.ttf font" << endl;
        exit(1);
    }

    MenuButton btnPlay;
    MenuButton btnQuit;

    btnPlay.action = MenuButton::MenuAction::Play;
    btnQuit.action = MenuButton::MenuAction::Quit;

    btnPlay.text.setFont(font);
    btnQuit.text.setFont(font);

    btnPlay.text.setString("Play");
    btnQuit.text.setString("Quit");

    btnPlay.text.setCharacterSize(40);
    btnQuit.text.setCharacterSize(40);

    btnPlay.text.setFillColor(Color::White);
    btnQuit.text.setFillColor(Color::White);

    menuButtons.push_front(btnQuit);
    menuButtons.push_front(btnPlay);


    cout << "MenuState: init" << endl;

}

void MenuState::cleanup() {
    cout << "MenuState: cleanup" << endl;
}

void MenuState::pause() {
    cout << "MenuState: pause" << endl;
}

void MenuState::resume() {
    cout << "MenuState: resume" << endl;
}

void MenuState::handleEvents(cgf::Game* game) {

    screen = game->getScreen();
    Event event;

    while(screen->pollEvent(event)) {
        switch (event.type) {
        case Event::EventType::MouseMoved :
            updateMenuButtons(Vector2i(event.mouseMove.x, event.mouseMove.y));
            break;
        case Event::EventType::MouseButtonPressed :
            handleClick(Vector2i(event.mouseButton.x, event.mouseButton.y), game);
            break;
        case Event::EventType::Closed :
            game->quit();
        default : ;

        }
    }

    cout << "MenuState: handleEvents" << endl;
}

void MenuState::update(cgf::Game* game) {
    cout << "MenuState: update" << endl;
}

void MenuState::draw(cgf::Game* game) {

    screen = game->getScreen();

    if (!didOrganizeMenuButtons) {
        organizeMenuButtons(screen->getSize().x);
        didOrganizeMenuButtons = true;
    }

    for (auto &button : menuButtons) {
        screen->draw(button.rect);
        screen->draw(button.text);
    }

    cout << "MenuState: draw" << endl;
}

void MenuState::organizeMenuButtons(int windowWidth) {

    float width = 250;
    float height = 100;
    float spacing = 30;

    float y = 200;
    float x = windowWidth/2 - width/2;

    for (auto &button : menuButtons) {

        button.bounds.top = y;
        button.bounds.left = x;

        button.bounds.height = height;
        button.bounds.width = width;

        button.rect.setPosition(x, y);
        button.rect.setSize(Vector2f(width, height));
        button.rect.setOutlineColor(Color::White);
        button.rect.setOutlineThickness(3);
        button.rect.setFillColor(Color::Transparent);

        FloatRect textBounds = button.text.getLocalBounds();

        button.text.setPosition(windowWidth/2 - textBounds.width/2, y + height/2 - textBounds.height/2 - 10);

        y = y + spacing + height;

    }
}

void MenuState::updateMenuButtons(Vector2i mousePos) {

    for (auto &button : menuButtons) {
        if (button.bounds.contains(mousePos))
            button.state = MenuButton::Hovering;
        else
            button.state = MenuButton::Unselected;

        switch(button.state) {
        case MenuButton::Unselected :
            button.text.setFillColor(Color::White);
            button.rect.setFillColor(Color::Transparent);
            break;
        case MenuButton::Hovering :
            button.text.setFillColor(Color::Black);
            button.rect.setFillColor(Color::White);
            break;
        default: ;
        }
    }
}

void MenuState::handleClick(Vector2i mousePos, cgf::Game* game) {

    int x = mousePos.x;
    int y = mousePos.y;

    for(auto &button : menuButtons) {

        if (button.bounds.contains(x, y)) {
            switch(button.action) {
            case MenuButton::MenuAction::Play :
//                changeState(game, PlayState::instance()));
                break;
            case MenuButton::MenuAction::Quit :
                game->quit();

            }
        }

    }

}
