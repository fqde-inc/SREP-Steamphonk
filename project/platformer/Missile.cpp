//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <cmath>
#include "Missile.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include "PlatformerGame.hpp"
#include "CharacterController.hpp"
#include "EnemyComponent.hpp"
#include "Damagable.hpp"

using namespace std;

Missile::Missile(GameObject *gameObject) : Component(gameObject) {

    gameObject->name = "Missile";

    missilePhysics = gameObject->addComponent<PhysicsComponent>();
    auto physicsScale = PlatformerGame::instance->physicsScale;
    
    radius = 7/physicsScale;

    missilePhysics->initCircle(b2_kinematicBody, radius, gameObject->getPosition()/physicsScale, 0);
    missilePhysics->setAutoUpdate(false);
    missilePhysics->setSensor(true);
}

void Missile::update(float deltaTime) {

    // Check whether missile has collided with the level using a raycast (platforms)
    auto from = missilePhysics->getBody()->GetWorldCenter();
    b2Vec2 to {from.x,from.y -radius*1.3f};
    PlatformerGame::instance->world->RayCast(this, from, to);

    //TODO: Do we want self desintegration ?
    lifetime += deltaTime;
    if(lifetime >= lifespan ) {
        gameObject->setConsumed( true );
    }

    gameObject->setPosition( gameObject->getPosition() + ( direction * constSpeed ));
    missilePhysics->moveTo( gameObject->getPosition()/PlatformerGame::instance->physicsScale);

}

// Raycast callback
float32 Missile::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    //TODO: Remove comment for wall & floor collisions

    //gameObject->setConsumed(true);
    return 0;
};

void Missile::onCollisionStart(PhysicsComponent *comp) {
    //TODO add collision handling on player's side
    auto go = comp->getGameObject();

    if ( go->name != origin ){
		
        if (comp->getGameObject()->getComponent<Damagable>() != nullptr)
        {
			comp->getGameObject()->getComponent<Damagable>()->takeDamage(damage);
        }
        
        gameObject->setConsumed(true);
    }
}

void Missile::onCollisionEnd(PhysicsComponent *comp) {
}