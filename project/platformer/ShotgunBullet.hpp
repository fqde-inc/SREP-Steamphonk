//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Box2D/Box2D.h"
#include "Bullet.hpp"

#pragma once

class ShotgunBullet : public Bullet{
public:
    ShotgunBullet(GameObject* gameObject) : Bullet(gameObject) {};
};
