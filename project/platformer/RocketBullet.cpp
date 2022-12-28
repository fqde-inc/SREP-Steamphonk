//
// Created by Giorgio Perri  on 31/10/22.
//

#include <Box2D/Box2D.h>
#include "PlatformerGame.hpp"
#include "Bullet.hpp"
#include "Explosion.hpp"

RocketBullet::RocketBullet(GameObject* gameObject) : Bullet(gameObject) {
    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("Projectile_0.png");
    spriteComponent->setSprite( sprite );
};

void RocketBullet::explode() {

    auto go = PlatformerGame::instance->createGameObject();
    go->setPosition(gameObject->getPosition());

    auto explosion = go->addComponent<Explosion>();
}

//void RocketBullet::update(float deltaTime) {
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
