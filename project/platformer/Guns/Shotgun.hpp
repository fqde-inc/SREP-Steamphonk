//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include <iostream>
#include "../PlayerShooting.hpp"

class ShotgunGun : public Gun {
public:
    explicit ShotgunGun() : Gun() {
        gunType = Shotgun;
        RecoilMagnitude = 0.1f;
    }

    void Fire(PlayerShooting &pShooting) override {
        pShooting.shootAtCursor();
    }
};
