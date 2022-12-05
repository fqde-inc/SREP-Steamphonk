//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Bullet.hpp"

#pragma once

class RocketBullet : Bullet{
public:
    explicit RocketBullet() {
        bulletType = Rocket;
    };
};
