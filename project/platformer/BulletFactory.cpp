
#include "BulletFactory.hpp"
#include "PlatformerGame.hpp"
#include "Bullet.hpp"
#include "RocketBullet.hpp"
#include "ShotgunBullet.hpp"
#include "GameObject.hpp"

std::shared_ptr<Bullet> BulletFactory::Make( BulletTypes type ){

    auto go = PlatformerGame::instance->createGameObject();

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
}