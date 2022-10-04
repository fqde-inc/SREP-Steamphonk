#pragma once

#include <Box2D/Box2D.h>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "Component.hpp"

class Laser : public Component, b2RayCastCallback{
public:
    explicit Laser(GameObject *gameObject);

    void init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic);

    void update(float deltaTime) override;

    float lifetime = 0;
    float lifespan = 2.5f;

    const glm::vec2 & getDirection() const{return direction;};
    void setDirection(const glm::vec2 & _direction) {
        direction = _direction; 
    };

    // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;

private:
    std::shared_ptr<PhysicsComponent> laserPhysics;
    glm::vec2 direction;

    float constSpeed = 5.5f;
    float radius;
};