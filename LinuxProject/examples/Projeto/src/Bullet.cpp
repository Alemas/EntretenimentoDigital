#include "Bullet.h"
#include "GameplayState.h"

#define BULLET_SPEED 1000

using namespace sf;
using namespace std;
using namespace cgf;

Bullet::Bullet(int damage, Vector2f position, sf::Vector2f direction) {

    this->damage = damage;
    sprite.load("data/img/survivor/bullet.png");
    sprite.scale(0.5, 0.5);
    physics = cgf::Physics::instance();

    sprite.setOrigin(sprite.getSize().x/2.0, sprite.getSize().y/2.0);
    body = physics->newCircle(GameplayState::BulletID, 0, 0, 5.0, 100, 0, 0, false);

    body->SetBullet(true);

    body->ApplyLinearImpulse(b2Vec2(direction.x*BULLET_SPEED, direction.y*BULLET_SPEED), body->GetWorldCenter(), true);

    physics->setImage(body, &sprite);
    physics->setPosition(body, b2Vec2(position.x, position.y));
}

Bullet::~Bullet()
{

}

