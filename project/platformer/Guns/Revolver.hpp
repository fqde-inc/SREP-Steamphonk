//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include <iostream>

class RevolverGun : public Gun {
public:
    explicit RevolverGun() : Gun() {
        gunType = Revolver;
    }

    void Fire() override {
        std::cout << "Bang!" << std::endl;
    }
};
