#include "HUD.h"

using namespace std;
using namespace sf;

HUD::HUD()
{

}

HUD::~HUD()
{

}


void HUD::init(Player* player) {

    this->player = player;

    clock = Clock();

     if (!font.loadFromFile("data/fonts/Sansation_Regular.ttf")) {
        cout << "Cannot load Sansation_Regular.ttf font" << endl;
        exit(1);
    }

    txtHP.setFont(font);
    txtAmmo.setFont(font);
    txtScore.setFont(font);
    txtmsg.setFont(font);

    txtHP.setCharacterSize(50);
    txtAmmo.setCharacterSize(100);
    txtScore.setCharacterSize(50);

    txtHP.setString("100");
    txtAmmo.setString(getAmmoString());
    txtScore.setString("100");

    txtScore.setFillColor(Color::Yellow);
    txtmsg.setFillColor(Color::Green);

    lastMsgTime = Time::Zero;
//    txt

}

string HUD::getAmmoString() {

    Player::Weapon w = player->getCurrentWeapon();

    if (!w.shoot)
        return "";

    string ammo = "";
    ammo = ammo + to_string(w.currentMagazine) + "/" + to_string(w.currentAmmo);

    return ammo;
}

void HUD::showMessage(string message, int fontSize, sf::Time time) {

    txtmsg.setString(message);
    txtmsg.setCharacterSize(fontSize);
    lastMsgTime = clock.getElapsedTime();
    msgTime = time;

}

void HUD::update(sf::RenderWindow* screen) {

    txtHP.setString(to_string(player->getHealth()));
    txtAmmo.setString(getAmmoString());
    txtScore.setString(L"100");

    Vector2f center = screen->getView().getCenter();
    Vector2f viewSize = screen->getView().getSize();
    Vector2f origin = Vector2f(center.x - viewSize.x/2, center.y - viewSize.y/2);

    txtHP.setPosition(Vector2f(origin.x + 20, origin.y + viewSize.y - 40 - txtHP.getLocalBounds().height));
    txtAmmo.setPosition(Vector2f(origin.x + viewSize.x - 50 - txtAmmo.getLocalBounds().width, origin.y + viewSize.y - 50 - txtAmmo.getLocalBounds().height));
    txtScore.setPosition(Vector2f(origin.x + viewSize.x - 50 - txtScore.getLocalBounds().width, origin.y + 20));

    txtmsg.setPosition(Vector2f(center.x - txtmsg.getLocalBounds().width/2, center.y));

    shouldDisplayMessage = (clock.getElapsedTime() - lastMsgTime) < msgTime;

}

void HUD::draw(sf::RenderWindow* screen) {

    update(screen);

    screen->draw(txtHP);
    screen->draw(txtAmmo);
    screen->draw(txtScore);

    if (shouldDisplayMessage)
        screen->draw(txtmsg);

}

