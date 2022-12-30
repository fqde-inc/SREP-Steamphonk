#pragma once

#include "Projectile.hpp"
#include "GameObject.hpp"

enum BulletTypes {
    None,
    Regular,
    Rocket
};

class Bullet : public Projectile {
public:
    explicit Bullet(GameObject* gameObject);
};

class RegularBullet : public Bullet{
public:
    RegularBullet(GameObject* gameObject) : Bullet(gameObject) {
        speed = 5.5f;
        damage = 1;
    };
};

class RocketBullet : public Bullet {
public:

    RocketBullet(GameObject* gameObject);

    // raycast callback
    float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) {
        if( Bullet::ReportFixture(fixture, point, normal, fraction) )
            return 1;
            
        explode();
        return 0;
    };

    void explode();

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;
    
    void update(float deltaTime) override;
private:
    bool mustExplode = false;
};
