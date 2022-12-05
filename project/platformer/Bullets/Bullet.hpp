//
// Created by Giorgio Perri  on 31/10/22.
//

#pragma once

enum BulletTypes {
    None,
    Shotgun,
    Rocket
};

class Bullet {
public:
    explicit Bullet();
    BulletTypes bulletType = None;
};
