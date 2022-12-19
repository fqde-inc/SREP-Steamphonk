//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include <iostream>
#include "../PlayerShooting.hpp"

enum GunTypes {
    NullGun,
    Shotgun,
    RocketLauncher,
};

class Gun {
public:
    explicit Gun() {}

    virtual void Fire(PlayerShooting &pShooting) {
        std::cout << "Whiff!" << std::endl;
    }

    GunTypes gunType = NullGun;
};