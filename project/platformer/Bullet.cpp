//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Bullet.hpp"
#include "PlatformerGame.hpp"

Bullet::Bullet(GameObject* gameObject): Missile(gameObject) {
    b2Filter filter = missilePhysics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::BULLET;
    filter.maskBits     = PlatformerGame::MISSILE | PlatformerGame::ENEMY | PlatformerGame::WALLS;
    missilePhysics->getFixture()->SetFilterData(filter);
};

