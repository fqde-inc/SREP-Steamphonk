//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Box2D/Box2D.h"
#include "Bullet.hpp"

#pragma once

class RegularBullet : public Bullet{
public:
    RegularBullet(GameObject* gameObject) : Bullet(gameObject) {};
};
