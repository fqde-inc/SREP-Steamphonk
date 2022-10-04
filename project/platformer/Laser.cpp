//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <cmath>
#include "Laser.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include "PlatformerGame.hpp"
#include "CharacterController.hpp"

using namespace std;


Laser::Laser(GameObject *gameObject) : Component(gameObject) {
    laserPhysics = gameObject->addComponent<PhysicsComponent>();
    auto physicsScale = PlatformerGame::instance->physicsScale;
    radius = 7/physicsScale;

    laserPhysics->initCircle(b2_kinematicBody, radius, glm::vec2{1,1} * Level::tileSize/physicsScale, 0);
    laserPhysics->setAutoUpdate(false);
}

void Laser::update(float deltaTime) {
    //TODO: Do we want self desintegration ?
    lifetime += deltaTime;
    if(lifetime >= lifespan ) {
        gameObject->setConsumed( true );
    }

    gameObject->setPosition( gameObject->getPosition() + ( direction * constSpeed ));
    laserPhysics->moveTo(gameObject->getPosition()/PlatformerGame::instance->physicsScale);

}

void Laser::onCollisionStart(PhysicsComponent *comp) {
    cout << "name : " << comp->getGameObject()->name << "\n";
    if ( comp->getGameObject()->name == "Player" ){
        comp->addImpulse( direction );
        gameObject->setConsumed(true);
    } else if ( comp->getGameObject()->name == "Platform"){
        gameObject->setConsumed(true);
    }
}

void Laser::onCollisionEnd(PhysicsComponent *comp) {
    gameObject->setConsumed(true);
}