#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <cmath>
#include "Projectile.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include "PlatformerGame.hpp"
#include "CharacterController.hpp"
#include "EnemyComponent.hpp"
#include "Damagable.hpp"

using namespace std;

Projectile::Projectile(GameObject *gameObject) : Component(gameObject) {

    gameObject->name = "Projectile";

    physics = gameObject->addComponent<PhysicsComponent>();
    auto physicsScale = PlatformerGame::instance->physicsScale;

    physics->initCircle(b2_kinematicBody, radius/physicsScale, gameObject->getPosition()/physicsScale, 0);
    physics->setAutoUpdate(false);
    physics->setSensor(true);

    b2Filter filter = physics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::MISSILE;
    filter.maskBits     = PlatformerGame::BULLET | PlatformerGame::WALLS | PlatformerGame::EXPLOSIONS | PlatformerGame::PLAYER;
    physics->getFixture()->SetFilterData(filter);

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("projectile.png");
	sprite.setOrderInBatch(16);
    spriteComponent = gameObject->addComponent<SpriteComponent>();
    spriteComponent->setSprite(sprite);
}

void Projectile::update(float deltaTime) {
    
    // Self desintegration ?
    lifetime += deltaTime;
    if(lifetime >= lifespan ) {
        gameObject->setConsumed( true );
    }

    // Check whether Projectile has collided with the ground using a raycast (platforms)
    auto from = physics->getBody()->GetPosition();

    // We want the raycast to go in the facing direction
    // We use cos(abs(angle)) for x, so regardless of the facing angle
    // It's always forward
    float angle = atan2(direction.y, direction.x);
    b2Vec2 facing(cos(abs(angle)), sin(angle));

    // Y axis uses that angle too, minus radius size to not over extend body size
    if (angle < 0.0f)
        facing.y = facing.y - radius/PlatformerGame::instance->physicsScale;

    // shorting length of raycast
    b2Vec2 to = from + 0.1f * facing;

    PlatformerGame::instance->world->RayCast(this, from, to);

    // Physics update
    gameObject->setPosition( gameObject->getPosition() + ( direction * speed ));
    physics->moveTo( gameObject->getPosition()/PlatformerGame::instance->physicsScale);
}

// Raycast callback
float32 Projectile::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    if(fixture->GetFilterData().categoryBits != PlatformerGame::WALLS)
    {
        return 1;
    }

    Mix_PlayChannel(-1, PlatformerGame::instance->hitWallSFX, 0);

    fixture->GetBody()->ApplyLinearImpulseToCenter(b2Vec2{2.0f,2.0f}, true);
    gameObject->setConsumed(true);
    return 0;
};

void Projectile::onCollisionStart(PhysicsComponent *comp) {
    //TODO add collision handling on player's side
    auto go = comp->getGameObject();

    if ( go->name == origin )
		return;
        
    if (go->getComponent<Damagable>() != nullptr) {
        go->getComponent<Damagable>()->takeDamage(damage);
    }
    
    gameObject->setConsumed(true);
}

void Projectile::onCollisionEnd(PhysicsComponent *comp) {
}