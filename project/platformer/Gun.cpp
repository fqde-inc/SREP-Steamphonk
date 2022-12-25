//
// Created by Giorgio Perri  on 31/10/22.
// 
#include <iostream>
#include "Box2D/Box2D.h"
#include "Gun.hpp"
#include "Bullet.hpp"
#include "SpriteComponent.hpp"
#include "PlatformerGame.hpp"


void Gun::Fire(glm::vec2 _position, glm::vec2 _direction) {

    auto l = BulletFactory::Make(bulletType);

    l->setOrigin("Player");
    l->setDirection(_direction);

    auto go = l->getGameObject();
    go->setPosition(_position);
    go->setRotation(180 - glm::atan(_direction.x, _direction.y) * 180 / M_PI);
}