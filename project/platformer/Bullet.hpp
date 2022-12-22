//
// Created by Giorgio Perri  on 31/10/22.
//
#pragma once

#include "Box2D/Box2D.h"
#include "Missile.hpp"
#include "GameObject.hpp"


class Bullet : public Missile {
public:
    Bullet(GameObject* gameObject) : Missile(gameObject) {};

    int damage = 10;
    int speed = 500;

    glm::vec2 velocity;
    glm::vec2 acceleration;
};
