#include "Enemy.h"
#include "GameplayState.h"

#define VECTOR_ZERO Vector2i(0,0)

using namespace sf;
using namespace std;
using namespace cgf;

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
    //dtor
}

void Enemy::init() {

    clock = Clock();

    Time zero = Time().Zero;
    Time coolDown = seconds(2.0);

    updateState(Top_Idle);

    physics = cgf::Physics::instance();

    body = physics->newCircle(GameplayState::BodyID::EnemyID, 0, 0, 12.5, 400, 0.5, 1.0, false);
    physics->setImage(body, &topSprite);
    body->SetFixedRotation(false);
    body->SetLinearDamping(10);
}

void Enemy::setPosition(b2Vec2 pos) {
    physics->setPosition(body, pos);
}

void Enemy::meleeAttack() {

}

void Enemy::draw(RenderWindow* screen) {
    screen->draw(topSprite);
}

void Enemy::update(float updateTimeInterval) {
    topSprite.update(updateTimeInterval, false);
}

void Enemy::updateMovement(sf::Vector2i lookingPoint, sf::Vector2i moveDirection, bool sprint) {

    b2Vec2 bodyPos = physics->getPosition(body);
    Vector2f pos = Vector2f(bodyPos.x, bodyPos.y);

    lookingLine[0].position = pos;
    lookingLine[0].color = Color::Yellow;
    lookingLine[1].position = Vector2f(lookingPoint.x, lookingPoint.y);
    lookingLine[1].color = Color::Yellow;

    float topRotation = Calculator::angleBetweenPoints(pos, Vector2f(lookingPoint.x, lookingPoint.y));
    body->SetTransform(body->GetPosition(), topRotation);

//    topRotation = Calculator::toDegrees(topRotation);
//    topSprite.setRotation(topRotation);

    TopState s = Top_Idle;

    if (moveDirection == VECTOR_ZERO) {
        s = Top_Idle;
    } else {
        if (sprint) {
            body->ApplyLinearImpulse(b2Vec2(moveDirection.x*runSpeed, moveDirection.y*runSpeed), body->GetWorldCenter(), true);
            s = Top_Run;
        } else {
            body->ApplyLinearImpulse(b2Vec2(moveDirection.x*walkSpeed, moveDirection.y*walkSpeed), body->GetWorldCenter(), true);
            s = Top_Walk;
        }
    }

    updateState(s);
//    cout << moveDirection.x << ", " << moveDirection.y << endl;
}

void Enemy::updateState(TopState state) {

}

