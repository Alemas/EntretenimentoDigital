#include "Bullet.h"
#include "GameplayState.h"

using namespace sf;
using namespace std;
using namespace cgf;

Bullet::Bullet(int damage, Vector2f position) {

    damage = damage;
    sprite.load("data/img/survivor/bullet.png");
    physics = cgf::Physics::instance();

    sprite.setOrigin(sprite.getSize().x/2.0, sprite.getSize().y/2.0);
    body = physics->newCircle(GameplayState::BulletID, 0, 0, 10.0, 100, 0, 0, false);
    body->SetBullet(true);

    physics->setImage(body, &sprite);
    physics->setPosition(body, b2Vec2(position.x, position.y));
}

Bullet::~Bullet()
{
    //dtor
}

