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
    static std::shared_ptr<Bullet> Make(std::shared_ptr<GameObject> go, BulletTypes type){
        switch (type)
        {
            case None:
                return go->addComponent<Bullet>();
            case Pellets:
                return go->addComponent<ShotgunBullet>();
            case Rocket:
                return go->addComponent<RocketBullet>();
            default:
                return go->addComponent<RocketBullet>();
        }
    };
};