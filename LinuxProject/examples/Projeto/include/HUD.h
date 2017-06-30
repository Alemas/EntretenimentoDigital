#ifndef HUD_H
#define HUD_H

#include <Player.h>

#include <iostream>
#include <string>

class HUD
{
    public:
        HUD();
        virtual ~HUD();
        void init(Player* player);
        void draw(sf::RenderWindow* screen);
        void showMessage(std::string message, int fontSize, sf::Time time);

    protected:

    private:

    std::string getAmmoString();
    void update(sf::RenderWindow* screen);

    bool shouldDisplayMessage;

    Player* player;
    sf::Font font;
    sf::Text txtAmmo;
    sf::Text txtHP;
    sf::Text txtScore;
    sf::Text txtmsg;

    sf::Clock clock;
    sf::Time msgTime;
    sf::Time lastMsgTime;
};

#endif // HUD_H
