#pragma once

#include "GameObject.hpp"
#include "Component.hpp"

class Laser : public Component {
public:
    explicit Laser(GameObject *gameObject);
    void init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic);

    void update(float deltaTime) override;

    float lifetime = 0;
    bool wasted = false;
private:
    float constSpeed = 8.0f;
};