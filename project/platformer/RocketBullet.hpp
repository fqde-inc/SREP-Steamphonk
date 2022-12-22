//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Bullet.hpp"

#pragma once

class RocketBullet : public Bullet {
public:

    float explosionRadius;
    int splashDamage;
    float steer_force = 15.0f;

    RocketBullet(GameObject* gameObject) : Bullet(gameObject){};


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
};
