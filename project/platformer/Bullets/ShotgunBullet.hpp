//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Bullet.hpp"

#pragma once

class ShotgunBullet : Bullet{
public:
    explicit ShotgunBullet() {
        bulletType = Shotgun;
    };
};
