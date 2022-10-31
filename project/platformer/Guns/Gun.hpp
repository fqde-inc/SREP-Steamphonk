//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include <iostream>

enum GunTypes {
    NullGun,
    Revolver,
};

class Gun {
public:
    explicit Gun() {}

    virtual void Fire() {
        std::cout << "Whiff!" << std::endl;
    }

    GunTypes gunType = NullGun;
};