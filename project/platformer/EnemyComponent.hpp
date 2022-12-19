#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include <vector>
#include "glm/glm.hpp"
#include "PhysicsComponent.hpp"
#include "FollowPathComponent.hpp"

class EnemyComponent : public Component, public b2RayCastCallback {
public:
    explicit EnemyComponent(GameObject *gameObject);

    void update(float deltaTime) override;

    glm::vec2 computePositionAtTime(float time);
    
    void kill();
    
    void setPathing( std::vector<glm::vec2> positions, PathType type);
    std::shared_ptr<FollowPathComponent> getPathing() {return path;};

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;
    float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) override;
private:

    std::shared_ptr<FollowPathComponent> path;
    std::shared_ptr<PhysicsComponent> physics;

    void shootAtPlayer();
    void animate();

    bool isAlive = true;
    bool mustDie = false;
    glm::vec2 deathForce;
    
    glm::vec2* target;

    float keyFrameTime = 1.0f;

    float reloadTime = 0.0f;
    float reloadTimeLimit = 2.5f;
    float shootingInterval = 0.15f;

    float keepAlive = 0.0f;
    float keepAliveTimeLimit = 2.5f;

    int shots = 3;
    int shotsRemaining = 3;

    float radius;
};

