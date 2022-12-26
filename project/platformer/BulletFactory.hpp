#pragma once 
#include "Bullet.hpp"

enum BulletTypes {
    None,
    Pellets,
    Rocket
};

class BulletFactory {
public:
    static std::shared_ptr<Bullet> Make( BulletTypes type );
};