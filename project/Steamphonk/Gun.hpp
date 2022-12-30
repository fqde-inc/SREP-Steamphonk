#pragma once

#include <iostream>
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
    float cooldownTime = .15f;
    
    bool Fire(glm::vec2 _position, glm::vec2 _direction);
    void reload(){clipSize = maxClipSize;};
};

class HandgunGun : public Gun {
public:
    explicit HandgunGun();
};

class RocketLauncherGun : public Gun {
public:
    explicit RocketLauncherGun();
};