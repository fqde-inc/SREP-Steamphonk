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
    }

    void Fire(PlayerShooting &pShooting) override {
        std::cout << "Rocket Bang!" << std::endl;
        pShooting.shootAtCursor();
    }
};
