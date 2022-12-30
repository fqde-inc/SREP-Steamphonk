//
// Created by Morten Nobel-Jørgensen, Clovis Lebret, Søren Skouv, Giorgio Perri on 11/6/17.
//

#include <Box2D/Box2D.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include "EnemyComponent.hpp"
#include "GameObject.hpp"
#include "PlatformerGame.hpp"
#include "Projectile.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "FollowPathComponent.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Damageable.hpp"

EnemyComponent::EnemyComponent(GameObject *gameObject) : Component(gameObject) {
    physics = gameObject->addComponent<PhysicsComponent>();
    auto physicsScale = PlatformerGame::instance->physicsScale;
    physics->initCircle(b2_dynamicBody, 15/physicsScale, gameObject->getPosition()/physicsScale, 0);
    physics->setAutoUpdate(false);
    physics->getBody()->SetGravityScale(0.0f);
    physics->setSensor(true);

    b2Filter filter = physics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::ENEMY;
    filter.maskBits     = PlatformerGame::BULLET | PlatformerGame::PLAYER | PlatformerGame::EXPLOSIONS;
    physics->getFixture()->SetFilterData(filter);

    damagable = gameObject->addComponent<Damageable>();
    damagable->setMaxLife(1);
    damagable->setLife(1);
    damagable->overrideDamageSound([]() {
        Mix_PlayChannel(-1, PlatformerGame::instance->hitBirdSFX, 0);
    });
}

void EnemyComponent::setPathing( std::vector<glm::vec2> positions, PathType type = BEZIER){
    path = gameObject->addComponent<FollowPathComponent>();
    path->setPositions(positions);
    path->setType(type);
}

void EnemyComponent::update(float deltaTime) {

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
}

void EnemyComponent::shootAtPlayer(){

    glm::vec2 direction = glm::normalize( PlatformerGame::instance->getPlayerPositon() - gameObject->getPosition() );

    auto go = PlatformerGame::instance->createGameObject();     
    go->setPosition(gameObject->getPosition());

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("projectile.png");
    auto spriteComponent = go->addComponent<SpriteComponent>();
    spriteComponent->setSprite( sprite );

    auto l = go->addComponent<Projectile>();
    l->setDirection(direction);
    l->setOrigin(gameObject->name);

    go->setRotation( 180 - glm::atan(direction.x, direction.y) * 180 / M_PI );
}

void EnemyComponent::onCollisionStart(PhysicsComponent *comp) {

    if( comp->getGameObject()->getComponent<Projectile>() != nullptr ){
        std::shared_ptr<Projectile> projectile = comp->getGameObject()->getComponent<Projectile>();
    }
}

void EnemyComponent::onCollisionEnd(PhysicsComponent *comp) {
}

float32 EnemyComponent::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    return 0;
}