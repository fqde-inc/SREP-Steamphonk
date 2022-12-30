#pragma once

#include <Box2D/Box2D.h>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "Component.hpp"

class Projectile : public Component, b2RayCastCallback{
public:
    Projectile(GameObject* gameObject);

    void update(float deltaTime) override;;
    void setOrigin(std::string _origin) { origin = _origin;};;
    void setDirection(const glm::vec2 & _direction) { direction = _direction; };

    // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;

protected:
    std::shared_ptr<PhysicsComponent> physics;
    std::shared_ptr<SpriteComponent> spriteComponent;
    glm::vec2 direction;

    float lifetime = 0;
    float lifespan = 2.5f;

    float speed = 3.5f;
    float radius = 7.0f;

    float damage = 1;

    std::string origin;
};