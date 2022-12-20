//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include <iostream>
#include "../PlayerShooting.hpp"

class RocketLauncherGun : public Gun {
public:
    explicit RocketLauncherGun() : Gun() {
        gunType = RocketLauncher;
        RecoilMagnitude = 0.2f;
    }

    void Fire(PlayerShooting &pShooting) override {
        pShooting.shootAtCursor();
    }
};
