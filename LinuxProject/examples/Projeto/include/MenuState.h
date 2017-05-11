#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <iostream>
#include <list>

#include "GameState.h"
#include "GameplayState.h"

class MenuState : public cgf::GameState
{
    public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);

    static MenuState* instance() {
        return &m_MenuState;
    }

    protected:

    MenuState() {}

    private:

    static MenuState m_MenuState;

    struct MenuButton {
        enum ButtonState { Selected, Unselected, Hovering };
        enum MenuAction { Play, Quit };
        sf::Rect<int> bounds;
        sf::RectangleShape rect;
        sf::Text text;
        MenuAction action;
        ButtonState state = Unselected;
    };

    sf::Font font;
    sf::RenderWindow* screen;

    std::list<MenuButton> menuButtons;
    bool didOrganizeMenuButtons = false;

    void organizeMenuButtons(sf::Vector2u windowSize);
    void updateMenuButtons(sf::Vector2i mousePos);
    void handleClick(sf::Vector2i mousePos, cgf::Game* game);

};

#endif // MENUSTATE_H
