//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once

#include <iostream>
#include "Box2D/Box2D.h"
#include "BulletFactory.hpp"
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

    int maxClipSize = 3;
    int clipSize;
    
    std::shared_ptr<TimerComponent> cooldown;
    float cooldownTime = .1f;
    
    bool Fire(glm::vec2 _position, glm::vec2 _direction);
    void reload(){clipSize = maxClipSize;};
};