#include "PlatformerGame.hpp"
#include "Bullet.hpp"
#include "Explosion.hpp"

RocketBullet::RocketBullet(GameObject* gameObject) : Bullet(gameObject) {
    speed = 5.5f;
    damage = 3;

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("Projectile_0.png");
    spriteComponent->setSprite( sprite );
};

void RocketBullet::explode() {
    auto go = PlatformerGame::instance->createGameObject();
    if( gameObject->getPosition() != glm::vec2{0} )
        go->setPosition(gameObject->getPosition());
    else 
        go->setPosition( glm::vec2 {
            physics->getBody()->GetPosition().x,
            physics->getBody()->GetPosition().y
        });

    auto explosion = go->addComponent<Explosion>();

    PlatformerGame::instance->setScreenshake(PlatformerGame::instance->shakeOption);
}

void RocketBullet::onCollisionStart(PhysicsComponent *comp) {
    Bullet::onCollisionStart(comp);

    if( comp->getFixture()->GetFilterData().categoryBits == PlatformerGame::ENEMY ){
        // Box2D doesnt't like to create new bodies during collision
        // Delay explosion post-collision by 1 extra frame so it can be generated during update
        gameObject->setConsumed(false);
        mustExplode = true;
    }
};

void RocketBullet::onCollisionEnd(PhysicsComponent *comp) {
    //explode();
};

void RocketBullet::update(float deltaTime) {
    if(mustExplode){
        explode();
        gameObject->setConsumed(true);
        return;
    } 
    Bullet::update(deltaTime);
}

float32 RocketBullet::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    if( Bullet::ReportFixture(fixture, point, normal, fraction) )
        return 1;

    explode();
    return 0;
}
