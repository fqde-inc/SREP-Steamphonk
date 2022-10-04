#pragma once

#include <Box2D/Box2D.h>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "Component.hpp"

class Laser : public Component {
public:
    explicit Laser(GameObject *gameObject);

    void init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic);

    void update(float deltaTime) override;

    float lifetime = 0;
    float lifespan = 2.0f;

    const glm::vec2 & getDirection() const{return direction;};
    void setDirection(const glm::vec2 & _direction) {
        direction = _direction; 
    };


    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;

private:
    std::shared_ptr<PhysicsComponent> laserPhysics;
    glm::vec2 direction;

    float constSpeed = 5.0f;
    float radius;
};