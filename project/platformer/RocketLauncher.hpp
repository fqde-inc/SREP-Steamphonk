//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include "BulletFactory.hpp"

class RocketLauncherGun : public Gun {
public:
    explicit RocketLauncherGun() {
        gunType = RocketLauncher;
        bulletType = Pellets;
        RecoilMagnitude = 0.2f;
    }
};