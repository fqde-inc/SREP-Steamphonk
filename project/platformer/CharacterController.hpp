#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "Damagable.hpp"
#include "sre/Sprite.hpp"
#include "CharacterControllerPDA.hpp"
#include "Gun.hpp"
#include "TimerComponent.hpp"
#include "Damagable.hpp"

using namespace std;
using namespace glm;

class CharacterState;

class CharacterController : public Component, public b2RayCastCallback {
public:
    explicit CharacterController(GameObject *gameObject);

    void updateSprite(float deltaTime);

    void update(float deltaTime) override;

    bool handleInput(SDL_Event &event) override;

    bool unlockedRocketLauncher = false;
    unique_ptr<Gun> rocketLauncher = make_unique<RocketLauncherGun>();

    bool unlockedHandgun = false;
    unique_ptr<Gun> handgun = make_unique<HandgunGun>();

    shared_ptr<Damagable> damageComponent;

    GunTypes equippedGun = Handgun;

    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    shared_ptr<PhysicsComponent> characterPhysics;
    shared_ptr<Damagable> characterDamagable;
    shared_ptr<TimerComponent> swapTimer;
    shared_ptr<TimerComponent> reloadTimer;
    float swapTime = .25f;
    float reloadTime = .75f;

    bool left = false;
    bool right = false;
    bool lastIsLeft = false;
    shared_ptr<CharacterState> state_;
    bool isGrounded = false;
    friend class CharacterState;
    friend class StandingState;
    friend class JumpingState;

    shared_ptr<SpriteComponent> spriteComponent;

    vec2 getShootDirection();

private:
    vec2 spawn;
    bool returnToSpawn = false;
    shared_ptr<GameObject> cursor;
    float radius;
};