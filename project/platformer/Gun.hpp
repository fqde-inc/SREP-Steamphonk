//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once

#include <iostream>
#include "Box2D/Box2D.h"
#include "Bullet.hpp"
#include "TimerComponent.hpp"


enum GunTypes {
    NullGun,
    Handgun,
    RocketLauncher,
};

class Gun {
public:
    explicit Gun();

    GunTypes gunType = NullGun;
    BulletTypes bulletType = None;
    float RecoilMagnitude = 0;

    int maxClipSize = 5;
    int clipSize;
    
    std::shared_ptr<TimerComponent> cooldown;
    float cooldownTime = .1f;
    
    bool Fire(glm::vec2 _position, glm::vec2 _direction);
    void reload(){clipSize = maxClipSize;};
};

class HandgunGun : public Gun {
public:
    explicit HandgunGun() {
        gunType = Handgun;
        bulletType = Regular;
        RecoilMagnitude = 0.001f;
    }
};

class RocketLauncherGun : public Gun {
public:
    explicit RocketLauncherGun() {
        gunType = RocketLauncher;
        bulletType = Rocket;
        RecoilMagnitude = 0.1f;

        maxClipSize = 1;
    }
};