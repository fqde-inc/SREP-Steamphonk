#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "sre/Sprite.hpp"
#include "CharacterControllerPDA.hpp"
#include "Guns/Gun.hpp"
#include "Guns/RocketLauncher.hpp"
#include "Guns/Shotgun.hpp"
#include "TimerComponent.hpp"
#include "Guns/RocketLauncher.hpp"
#include "PlayerShooting.hpp"


class CharacterState;

class CharacterController : public Component, public b2RayCastCallback {
public:
    explicit CharacterController(GameObject *gameObject);

    void setSprites(sre::Sprite standing,
                    sre::Sprite walk1,
                    sre::Sprite walk2,
                    sre::Sprite flyUp,
                    sre::Sprite fly,
                    sre::Sprite flyDown);

    void updateSprite(float deltaTime);

    void update(float deltaTime) override;

    bool handleInput(SDL_Event &event) override;

    std::unique_ptr<Gun> rocketLauncher = std::make_unique<RocketLauncherGun>();
    std::unique_ptr<Gun> shotgun = std::make_unique<ShotgunGun>();
    std::shared_ptr<PlayerShooting> playerShooting;

    GunTypes equippedGun = RocketLauncher;

    // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    std::shared_ptr<PhysicsComponent> characterPhysics;
    std::shared_ptr<TimerComponent> cooldownTimer;
    float cooldownTime = .75f;

    bool left = false;
    bool right = false;
    std::shared_ptr<CharacterState> state_;
    bool isGrounded = false;
    friend class CharacterState;
    friend class StandingState;
    friend class JumpingState;

    std::shared_ptr<SpriteComponent> spriteComponent;
private:
    sre::Sprite standing;
    sre::Sprite walk1;
    sre::Sprite walk2;
    sre::Sprite flyUp;
    sre::Sprite fly;
    sre::Sprite flyDown;
    std::shared_ptr<GameObject> cursor;
    float radius;

};