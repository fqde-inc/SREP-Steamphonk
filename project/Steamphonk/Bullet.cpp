#include "Bullet.hpp"
#include "PlatformerGame.hpp"

Bullet::Bullet(GameObject* gameObject): Projectile(gameObject) {
    b2Filter filter = physics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::BULLET;
    filter.maskBits     = PlatformerGame::MISSILE | PlatformerGame::ENEMY | PlatformerGame::WALLS;
    physics->getFixture()->SetFilterData(filter);
};

RegularBullet::RegularBullet(GameObject *gameObject) : Bullet(gameObject) {
    speed = 5.5f;
    damage = 1;
}
