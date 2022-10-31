//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once

#include "Gun.hpp"

class RevolverGun : Gun {
public:
    explicit RevolverGun() {
        gunType = Revolver;
    }

    void Fire() override;
};
