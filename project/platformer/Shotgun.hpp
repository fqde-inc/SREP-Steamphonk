//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include "BulletFactory.hpp"

class ShotgunGun : public Gun {
public:
    explicit ShotgunGun() {
        gunType = Shotgun;
        bulletType = Pellets;
        RecoilMagnitude = 0.15f;
    }
};