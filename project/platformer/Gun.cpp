//
// Created by Giorgio Perri  on 31/10/22.
// 

#include <map>
#include <memory>
#include <iostream>
#include "Box2D/Box2D.h"
#include "Gun.hpp"
#include "Bullet.hpp"
#include "SpriteComponent.hpp"
#include "TimerComponent.hpp"
#include "PlatformerGame.hpp"

Gun::Gun() {
    auto go = PlatformerGame::instance->createGameObject();
    cooldown = go->addComponent<TimerComponent>();
    reload();
}

bool Gun::Fire(glm::vec2 _position, glm::vec2 _direction) {

    if(clipSize == 0 || cooldown->isRunning)
        return false;

    auto go = PlatformerGame::instance->createGameObject();
    go->setPosition(_position);
    go->setRotation(180 - glm::atan(_direction.x, _direction.y) * 180 / M_PI);
    
    auto bullet = std::shared_ptr<Bullet>();

    if (bulletType == Regular) {
        bullet = go->addComponent<RegularBullet>();
    } else if (bulletType == Rocket) {
        bullet = go->addComponent<RocketBullet>();
    } else {
        bullet = go->addComponent<Bullet>();
    }

    bullet->setOrigin("Player");
    bullet->setDirection(_direction);

    clipSize--;
    cooldown->initTimer(cooldownTime);


    PlatformerGame::instance->setScreenshake(PlatformerGame::instance->currentShake);
    
    return true;
}