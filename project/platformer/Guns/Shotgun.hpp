//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include <iostream>

class ShotgunGun : public Gun {
public:
    explicit ShotgunGun() : Gun() {
        gunType = Shotgun;
    }

    void Fire() override {
        std::cout << "Shotgun Bang!" << std::endl;
    }
};
