//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//
#define GLM_ENABLE_EXPERIMENTAL = true

#include <Box2D/Box2D.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include "EnemyComponent.hpp"
#include "GameObject.hpp"
#include "PlatformerGame.hpp"
#include "Missile.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "FollowPathComponent.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Damagable.hpp"

EnemyComponent::EnemyComponent(GameObject *gameObject) : Component(gameObject) {
    physics = gameObject->addComponent<PhysicsComponent>();
    auto physicsScale = PlatformerGame::instance->physicsScale;

    physics->initCircle(b2_dynamicBody, 15/physicsScale, gameObject->getPosition()/physicsScale, 0);
    physics->setAutoUpdate(false);
    physics->getBody()->SetGravityScale(0.0f);
    physics->setSensor(true);

    damagable = gameObject->addComponent<Damagable>();
    damagable->setMaxLife(1);
    damagable->setLife(1);
}

void EnemyComponent::setPathing( std::vector<glm::vec2> positions, PathType type = BEZIER){
    path = gameObject->addComponent<FollowPathComponent>();
    path->setPositions(positions);
    path->setType(type);
}

void EnemyComponent::update(float deltaTime) {
    
    if(mustDie)
        kill();

    if(!isAlive){

        if(path != nullptr)
            gameObject->removeComponent(path);

        reloadTime += deltaTime;

        if ( reloadTime >= reloadTimeLimit ){
            gameObject->setConsumed(true);
        }
        return;
    }

    physics->moveTo(gameObject->getPosition()/PlatformerGame::instance->physicsScale);

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
    l->setOrigin(gameObject->name);

    go->setRotation( 180 - glm::atan(direction.x, direction.y) * 180 / M_PI );
}

void EnemyComponent::animate(){
    //TODO: Animate sprite ?
    //float t = fmod(time, keyFrameTime);
}

void EnemyComponent::kill(){
    isAlive = false;

    auto sprite = gameObject->getComponent<SpriteComponent>()->getSprite();
    sprite.setRotation(sprite.getRotation() + 180);
    gameObject->getComponent<SpriteComponent>()->setSprite(sprite);

    physics->getBody()->SetAwake(false);
    physics->getBody()->SetGravityScale(1);
    physics->getBody()->SetType(b2_kinematicBody);
    physics->setAutoUpdate(true);
    physics->addForce(lastHitDirection);
}

void EnemyComponent::onCollisionStart(PhysicsComponent *comp) {

    // std::cout << "Collision enemy : " << comp->getGameObject()->name << std::endl;
    if( comp->getGameObject()->getComponent<Missile>() != nullptr ){
        std::shared_ptr<Missile> missile = comp->getGameObject()->getComponent<Missile>();

        if( missile->getOrigin() == "Player" ){
            lastHitDirection = missile->getDirection();
        }
    }
}

void EnemyComponent::onCollisionEnd(PhysicsComponent *comp) {
}

float32 EnemyComponent::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    return 0;
}