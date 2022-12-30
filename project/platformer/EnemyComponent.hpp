#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include <vector>
#include "glm/glm.hpp"
#include "PhysicsComponent.hpp"
#include "FollowPathComponent.hpp"
#include "Damagable.hpp"

using namespace std;
using namespace glm;

class EnemyComponent : public Component, public b2RayCastCallback {
public:
    explicit EnemyComponent(GameObject *gameObject);

    void update(float deltaTime) override;
    
    void setPathing( vector<vec2> positions, PathType type);

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;
    float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) override;
private:

    shared_ptr<FollowPathComponent> path;
    shared_ptr<PhysicsComponent> physics;
    shared_ptr<Damagable> damagable;

    void shootAtPlayer();

    bool isAlive = true;
    
    vec2* target;

    float reloadTime = 0.0f;
    float reloadTimeLimit = 2.5f;
    float shootingInterval = 0.15f;

    int shotsRemaining = 3;

    float radius;
};

