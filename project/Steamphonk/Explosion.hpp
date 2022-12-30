#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "TimerComponent.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class Explosion : public Component, b2RayCastCallback {
public:
    explicit Explosion(GameObject* gameObject);
    void update(float deltaTime) override;

    void setRadius(float _radius){ radius = _radius; };

        // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;

    void setDamage(int _damage){damage = _damage;};
    void setKnockback(float _knockback){knockback = _knockback;};
    
private:

    shared_ptr<TimerComponent> timer;
    shared_ptr<SpriteComponent> spriteComponent;
    shared_ptr<PhysicsComponent> physics;
    
    float radius = 10.0f;
    float duration = 0.25f;
    
    vector<Sprite> animationSprites;
    int animationIndex = 0;
    float animationTime = 0;
    float animationFrameRate = 1.0f / 10.0f;
    
    int damage;
    float knockback;
};