//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once

enum GunTypes {
    NullGun,
    Revolver,
};

class Gun {
public:
    explicit Gun() {}
    virtual void Fire();
    GunTypes gunType = NullGun;
};