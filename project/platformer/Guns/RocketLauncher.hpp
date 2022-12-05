//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include <iostream>

class RocketLauncherGun : public Gun {
public:
    explicit RocketLauncherGun() : Gun() {
        gunType = RocketLauncher;
    }

    void Fire() override {
        std::cout << "Rocket Bang!" << std::endl;
    }
};
