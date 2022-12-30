//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Bullet.hpp"
#include "PlatformerGame.hpp"

Bullet::Bullet(GameObject* gameObject): Projectile(gameObject) {
    b2Filter filter = physics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::BULLET;
    filter.maskBits     = PlatformerGame::MISSILE | PlatformerGame::ENEMY | PlatformerGame::WALLS;
    physics->getFixture()->SetFilterData(filter);
};

