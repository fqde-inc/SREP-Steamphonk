//
// Created by Giorgio Perri  on 31/10/22.
//

#include "Bullet.hpp"
#include "PlatformerGame.hpp"

Bullet::Bullet(GameObject* gameObject): Missile(gameObject) {
    b2Filter filter = missilePhysics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::ENEMY;
    filter.maskBits     = PlatformerGame::BULLET | PlatformerGame::PLAYER;
    missilePhysics->getFixture()->SetFilterData(filter);
};


    // raycast callback
float32 Bullet::ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                    const b2Vec2& normal, float32 fraction){
    return 0;
}

void Bullet::onCollisionStart(PhysicsComponent *comp){
    return;
}

void Bullet::onCollisionEnd(PhysicsComponent *comp){
    return;
}
