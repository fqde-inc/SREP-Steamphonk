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
#include "PlayerShooting.hpp"

PlayerShooting::PlayerShooting(GameObject *gameObject) : Component(gameObject) {
    auto enemyPhysics = gameObject->addComponent<PhysicsComponent>();
}

void PlayerShooting::update(float deltaTime) {
    reloadTime += deltaTime;

    if ( reloadTime >= reloadTimeLimit ){
        if(shotsRemaining == 0){
            reloadTime = 0;
            shotsRemaining = 3;
        } else {
            reloadTime -= shootingInterval;
            shotsRemaining--;
        }
    }
}

bool PlayerShooting::handleInput(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_f:
        {
            shootAt(PlatformerGame::instance->crosshair->getPosition());
        }
    }

    return false;
}

void PlayerShooting::shootAt(glm::vec2 position)
{
    glm::vec2 direction = glm::normalize(position - gameObject->getPosition());

    auto go = PlatformerGame::instance->createGameObject();
    go->setPosition(gameObject->getPosition());

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("projectile.png");
    auto spriteComponent = go->addComponent<SpriteComponent>();
    spriteComponent->setSprite(sprite);

    auto l = go->addComponent<Missile>();
    l->setTarget("Bird");
    l->setDirection(direction);

    go->setRotation(180 - glm::atan(direction.x, direction.y) * 180 / M_PI);
}

void PlayerShooting::shootAtCursor()
{
    glm::vec2 direction = glm::normalize(PlatformerGame::instance->crosshair->getPosition() - gameObject->getPosition());

    auto go = PlatformerGame::instance->createGameObject();
    go->setPosition(gameObject->getPosition());

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("projectile.png");
    auto spriteComponent = go->addComponent<SpriteComponent>();
    spriteComponent->setSprite(sprite);

    auto l = go->addComponent<Missile>();
    l->setTarget("Bird");
    l->setDirection(direction);

    go->setRotation(180 - glm::atan(direction.x, direction.y) * 180 / M_PI);
}

glm::vec2 PlayerShooting::getShootDirection() {
    return glm::normalize(PlatformerGame::instance->crosshair->getPosition() - gameObject->getPosition());
}
