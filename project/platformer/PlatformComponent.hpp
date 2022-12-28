#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>
#include "glm/glm.hpp"

class GameObject;

class PlatformComponent : public Component {
public:
    explicit PlatformComponent(GameObject *gameObject);

    void initTile(std::shared_ptr<sre::SpriteAtlas> tileAtlas, std::pair<int,int> coords, std::string name);

    void moveTo(glm::vec2 tilePos);
    void update(float deltaTime) override;

    glm::vec2 getPosition();

    void onCollisionStart(PhysicsComponent *comp) override {};
    void onCollisionEnd(PhysicsComponent *comp) override {};
private:
    std::vector<std::shared_ptr<GameObject>> tiles;
    std::shared_ptr<PhysicsComponent> physics;
    bool kinematic;
    glm::vec2 pos;
    int width;
    float physicsScale;
};