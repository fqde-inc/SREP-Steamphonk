//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Bullet.hpp"

#pragma once

class RevolverBullet : Bullet{
public:
    explicit RevolverBullet() {
        bulletType = Revolver;
    };
};
