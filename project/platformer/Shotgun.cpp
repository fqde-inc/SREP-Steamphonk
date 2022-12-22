//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include <iostream>
#include "Shotgun.hpp"
#include "BulletFactory.hpp"
#include "SpriteComponent.hpp"
#include "PlatformerGame.hpp"


void ShotgunGun::Fire(glm::vec2 _position, glm::vec2 _direction) {
    auto go = PlatformerGame::instance->createGameObject();
    go->setPosition(_position);

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("projectile.png");
    auto spriteComponent = go->addComponent<SpriteComponent>();
    spriteComponent->setSprite(sprite);

    auto l = go->addComponent<Missile>();
    l->setOrigin("Player");
    l->setDirection(_direction);

    go->setRotation(180 - glm::atan(_direction.x, _direction.y) * 180 / M_PI);
    //return go;
}