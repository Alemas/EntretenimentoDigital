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

    protected:

    private:

    std::string getAmmoString();
    void update(sf::RenderWindow* screen);

    Player* player;
    sf::Font font;
    sf::Text txtAmmo;
    sf::Text txtHP;
    sf::Text txtScore;
};

#endif // HUD_H
