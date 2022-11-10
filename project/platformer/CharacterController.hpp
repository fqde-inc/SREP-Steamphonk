#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "sre/Sprite.hpp"
#include "CharacterControllerPDA.hpp"
#include "Guns/Gun.hpp"
#include "Guns/Revolver.hpp"
#include <tuple>

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

    std::tuple<std::shared_ptr<Gun>, std::shared_ptr<Gun>> equippedGuns = {std::make_shared<Gun>(), std::make_shared<RevolverGun>()};

    bool swappingGun = false;
    bool firing = false;

    // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    std::shared_ptr<PhysicsComponent> characterPhysics;
    bool left = false;
    bool right = false;
    std::shared_ptr<CharacterState> state_;
    bool isGrounded = false;
    friend class CharacterState;
    friend class StandingState;
    friend class JumpingState;
private:
    sre::Sprite standing;
    sre::Sprite walk1;
    sre::Sprite walk2;
    sre::Sprite flyUp;
    sre::Sprite fly;
    sre::Sprite flyDown;

    std::shared_ptr<SpriteComponent> spriteComponent;
    float radius;

};



