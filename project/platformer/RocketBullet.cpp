//
// Created by Giorgio Perri  on 31/10/22.
//

#include <Box2D/Box2D.h>
#include "PlatformerGame.hpp"
#include "Bullet.hpp"
#include "Explosion.hpp"

RocketBullet::RocketBullet(GameObject* gameObject) : Bullet(gameObject) {
    speed = 4.5f;
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
            ProjectilePhysics->getBody()->GetPosition().x,
            ProjectilePhysics->getBody()->GetPosition().y
        });

    auto explosion = go->addComponent<Explosion>();
    
    PlatformerGame::instance->shake = true;
}

void RocketBullet::onCollisionStart(PhysicsComponent *comp) {
    Bullet::onCollisionStart(comp);
    if( comp->getFixture()->GetFilterData().categoryBits == PlatformerGame::ENEMY ){
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
//    Bullet::update(deltaTime);

//     acceleration = SeekTarget();
//     velocity += acceleration * deltaTime;
//     velocity = velocity.clamped(speed)
//     rotation = velocity.angle()
//     position += velocity * delta * 100
//}

// void SeekTarget() {
//     if target:
//         auto desired = (target.position - position).normalized() * speed
//         steer = (desired - velocity).normalized() * steer_force
// }
