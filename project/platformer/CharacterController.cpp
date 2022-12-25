#include <SDL_events.h>
#include <iostream>
#include "CharacterController.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "PlatformerGame.hpp"
#include "SpriteComponent.hpp"
#include "PlayerShooting.hpp"
#include "Damagable.hpp"
using namespace std;

CharacterController::CharacterController(GameObject *gameObject) : Component(gameObject) {
    gameObject->name = "Player";

    characterPhysics = gameObject->addComponent<PhysicsComponent>();
    cooldownTimer = gameObject->addComponent<TimerComponent>();
    gameObject->addComponent<PlayerShooting>();

    auto physicsScale = PlatformerGame::instance->physicsScale;
    spawn = PlatformerGame::instance->getLevel()->getIdentifierPosition("PlayerStart");
   
    characterDamagable = gameObject->addComponent<Damagable>();
    characterDamagable->setMaxLife(10);
    characterDamagable->overrideDeathAction([this]() {
        returnToSpawn = true;
        });
	
    radius = 10/physicsScale;
    characterPhysics->initCircle(b2_dynamicBody, radius, spawn / physicsScale, 1);
    std::cout << "Spawned player at " << spawn.x << ", " << spawn.y << std::endl;
    characterPhysics->getFixture()->SetRestitution(0);
    characterPhysics->fixRotation();
    spriteComponent = gameObject->getComponent<SpriteComponent>();
    state_ = std::make_shared<CharacterState>();
}

bool CharacterController::handleInput(SDL_Event &event) {
    state_->handleInput(*this, event);
    return false;
}

void CharacterController::update(float deltaTime) {
	//Calling it here seems to be fine regarding isLocked so it doesn't happen during a simulation step
    if (returnToSpawn /*&& this->characterPhysics->getBody()->lock*/)
    {
        auto physicsScale = PlatformerGame::instance->physicsScale;
        this->characterPhysics->getBody()->SetTransform(b2Vec2(spawn.x / physicsScale, spawn.y / physicsScale), 0);
        this->characterDamagable->resetLife();
        std::cout << "Player returned to " << spawn.x << ", " << spawn.y << std::endl;
        returnToSpawn = false;
    }
	
    // raycast ignores any shape in the starting point
    auto from = characterPhysics->getBody()->GetWorldCenter();
    b2Vec2 to {from.x,from.y -radius*1.3f};
    isGrounded = false;
    PlatformerGame::instance->world->RayCast(this, from, to);

    characterPhysics->fixRotation();
    glm::vec2 movement{0,0};

    if (left){
        movement.x --;
    }
    if (right){
        movement.x ++;
    }

    glm::vec2 currentVel = characterPhysics->getLinearVelocity();

    if (currentVel.x > 0 && !right) {
        characterPhysics->setLinearVelocity(glm::vec2(currentVel.x - 0.15f, currentVel.y));
    }

    if (currentVel.x < 0 && !left) {
        characterPhysics->setLinearVelocity(glm::vec2(currentVel.x + 0.15f, currentVel.y));
    }

    if(!left && !right && glm::abs(currentVel.x) < 0.1f) {
        characterPhysics->setLinearVelocity(glm::vec2(0, currentVel.y));
    }

    float accelerationSpeed = 0.008f;
    characterPhysics->addImpulse(movement*accelerationSpeed);
    float maximumVelocity = 2.5f;
    auto linearVelocity = characterPhysics->getLinearVelocity();
    float currentVelocity = linearVelocity.x;
    if (abs(currentVelocity) > maximumVelocity){
        linearVelocity.x = glm::sign(linearVelocity.x)*maximumVelocity;
        characterPhysics->setLinearVelocity(linearVelocity);
    }
    updateSprite(deltaTime);

    if(state_->characterStateStack.size() != 0) state_->characterStateStack[0].get()->update(*this, deltaTime);
}

void CharacterController::onCollisionStart(PhysicsComponent *comp) {

}

void CharacterController::onCollisionEnd(PhysicsComponent *comp) {

}

void CharacterController::onDeath()
{
	auto spawn = PlatformerGame::instance->getLevel()->getIdentifierPosition("PlayerStart");
	gameObject->setPosition(spawn);
	characterPhysics->setLinearVelocity({ 0,0 });
}

float32 CharacterController::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    isGrounded = true;
    if(state_->characterStateStack[0]->stateType == Jumping) {
        state_->popStack(Jumping);
        if(left || right) {
            state_->pushStack(std::make_shared<WalkingState>());
        }
    }
    return 0;
}

void CharacterController::setSprites(sre::Sprite standing, sre::Sprite walk1, sre::Sprite walk2, sre::Sprite flyUp,
                                     sre::Sprite fly, sre::Sprite flyDown) {
    this->standing = standing;
    this->walk1 = walk1;
    this->walk2 = walk2;
    this->flyUp = flyUp;
    this->fly = fly;
    this->flyDown = flyDown;
}

void CharacterController::updateSprite(float deltaTime) {
    auto velocity = characterPhysics->getLinearVelocity();
    // todo implement
}


