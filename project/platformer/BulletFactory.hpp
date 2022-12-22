#pragma once
#include "Bullet.hpp"
#include "RocketBullet.hpp"
#include "ShotgunBullet.hpp"

enum BulletTypes {
    None,
    Pellets,
    Rocket
};

class BulletFactory {
public:
    std::shared_ptr<Bullet> Make(BulletTypes type){
        switch (type)
        {
            case None:
                return std::make_shared<Bullet>();
            case Pellets:
                return std::make_shared<Bullet>();
            case Rocket:
                return std::make_shared<Bullet>();
            default:
                return std::make_shared<Bullet>();
        }
    };
};