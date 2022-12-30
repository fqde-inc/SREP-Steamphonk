#pragma once

#include <Box2D/Box2D.h>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "Component.hpp"
#include "Gun.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class Collectible : public Component, b2RayCastCallback{
public:
    Collectible(GameObject* gameObject);

    void initCollectible(GunTypes gunType);
    void update(float deltaTime) override;

    // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    GunTypes gunType;
protected:
    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> physicsComponent;
    vector<Sprite> animationSprites;
    int animationIndex = 0;
    float animationTime = 0;
    float animationFrameRate = 1.0f / 10.0f;
};