//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include <iostream>
#include <stdio.h>
#include <cmath>
#include "EnemyComponent.hpp"
#include "GameObject.hpp"
#include "PlatformerGame.hpp"
#include "Missile.hpp"
#include "SpriteComponent.hpp"

//TODO refactor component to generic "pathFinder" 
EnemyComponent::EnemyComponent(GameObject *gameObject) : Component(gameObject) {
    auto enemyPhysics = gameObject->addComponent<PhysicsComponent>();
}

void EnemyComponent::update(float deltaTime) {
    reloadTime += deltaTime;

    if ( reloadTime >= reloadTimeLimit ){
        if(shotsRemaining == 0){
            reloadTime = 0;
            shotsRemaining = 3;
        } else {
            reloadTime -= shootingInterval;
            shotsRemaining--;
            shootAtPlayer();
        }
    }

    animate();
}

void EnemyComponent::shootAtPlayer(){

    glm::vec2 direction = glm::normalize( PlatformerGame::instance->getPlayerPositon() - gameObject->getPosition() );

    auto go = PlatformerGame::instance->createGameObject();     
    go->setPosition(gameObject->getPosition());

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("projectile.png");
    auto spriteComponent = go->addComponent<SpriteComponent>();
    spriteComponent->setSprite( sprite );

    auto l = go->addComponent<Missile>();
    l->setDirection(direction);

    go->setRotation( 180 - glm::atan(direction.x, direction.y) * 180 / M_PI );
    
}

void EnemyComponent::animate(){
    //TODO: Animate sprite ?
    //float t = fmod(time, keyFrameTime);
}
