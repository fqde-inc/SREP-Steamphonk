#pragma once 
#include "Bullet.hpp"

enum BulletTypes {
    None,
    Regular,
    Rocket
};

class BulletFactory {
public:
    static std::shared_ptr<Bullet> Make( BulletTypes type );
};