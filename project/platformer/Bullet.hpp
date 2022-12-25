//
// Created by Giorgio Perri  on 31/10/22.
//
#pragma once

#include "Missile.hpp"
#include "GameObject.hpp"

class Bullet : public Missile {
public:
    explicit Bullet(GameObject* gameObject);

    int damage = 10;
    int speed = 500;

    glm::vec2 velocity;
    glm::vec2 acceleration;

    // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;
};
