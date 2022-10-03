#pragma once

#include "GameObject.hpp"
#include "Component.hpp"

class Laser : public Component {
public:
    explicit Laser(GameObject *gameObject);
    void init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic);

    void update(float deltaTime) override;

    float lifetime = 0;
    float lifespan = 2.0f;
    bool wasted = false;

    const glm::vec2 & getDirection() const{return direction;};
    void setDirection(const glm::vec2 & _direction) {direction = _direction;};

private:
    glm::vec2 direction;
    float constSpeed = 4.0f;
};