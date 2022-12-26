//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once

#include <iostream>
#include "Box2D/Box2D.h"
#include "BulletFactory.hpp"


enum GunTypes {
    NullGun,
    Shotgun,
    RocketLauncher,
};

class Gun {
public:
    explicit Gun(){};

    GunTypes gunType = NullGun;
    BulletTypes bulletType = None;
    float RecoilMagnitude = 0;
    
    void Fire(glm::vec2 _position, glm::vec2 _direction);
};