#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "Damageable.hpp"
#include "sre/Sprite.hpp"
#include "CharacterControllerPDA.hpp"
#include "Gun.hpp"
#include "TimerComponent.hpp"
#include "Damageable.hpp"

class CharacterState;

class CharacterController : public Component, public b2RayCastCallback {
public:
    explicit CharacterController(GameObject *gameObject);

    void updateSprite(float deltaTime);

    void update(float deltaTime) override;

    bool handleInput(SDL_Event &event) override;

    bool unlockedRocketLauncher = false;
    std::unique_ptr<Gun> rocketLauncher = std::make_unique<RocketLauncherGun>();

    bool unlockedHandgun = false;
    std::unique_ptr<Gun> handgun = std::make_unique<HandgunGun>();

    std::shared_ptr<Damageable> damageComponent;

    GunTypes equippedGun = Handgun;

    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    std::shared_ptr<PhysicsComponent> characterPhysics;
    std::shared_ptr<Damageable> characterDamagable;
    std::shared_ptr<TimerComponent> swapTimer;
    std::shared_ptr<TimerComponent> reloadTimer;
    float swapTime = .25f;
    float reloadTime = .75f;

    bool left = false;
    bool right = false;
    bool lastIsLeft = false;
    std::shared_ptr<CharacterState> state_;
    bool isGrounded = false;
    friend class CharacterState;
    friend class StandingState;
    friend class JumpingState;

    std::shared_ptr<SpriteComponent> spriteComponent;

    glm::vec2 getShootDirection();

private:
    glm::vec2 spawn;
    bool returnToSpawn = false;
    sre::Sprite standing;
    sre::Sprite walk1;
    sre::Sprite walk2;
    sre::Sprite flyUp;
    sre::Sprite fly;
    sre::Sprite flyDown;
    std::shared_ptr<GameObject> cursor;
    float radius;
};