//
// Created by Giorgio Perri  on 31/10/22.
//
#pragma once
#include <Box2D/Box2D.h>
#include "PlatformerGame.hpp"
#include "Bullet.hpp"
#include "RocketBullet.hpp"

RocketBullet::RocketBullet(GameObject* gameObject) : Bullet(gameObject) {
    
    std::cout << "Fired rocket" << std::endl;

    missilePhysics->setAutoUpdate(false);

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("missile_0.png");
    spriteComponent->setSprite( sprite );
};

// Raycast callback
float32 RocketBullet::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    //TODO: Remove comment for wall & floor collisions

    gameObject->setConsumed(true);
    return 0;
};

void RocketBullet::onCollisionEnd(PhysicsComponent *comp) {
}
void RocketBullet::onCollisionStart(PhysicsComponent *comp) {
    Bullet::onCollisionStart(comp); 
}

// void update(float deltaTime) override {
//     acceleration = SeekTarget();
//     velocity += acceleration * deltaTime;
//     velocity = velocity.clamped(speed)
//     rotation = velocity.angle()
//     position += velocity * delta * 100
// }

// void SeekTarget() {
//     if target:
//         auto desired = (target.position - position).normalized() * speed
//         steer = (desired - velocity).normalized() * steer_force
// }
