//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once
#include "Gun.hpp"
#include "BulletFactory.hpp"

class HandgunGun : public Gun {
public:
    explicit HandgunGun() {
        gunType = Handgun;
        bulletType = Regular;
        RecoilMagnitude = 0.05f;
    }
};