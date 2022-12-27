//
// Created by Giorgio Perri  on 31/10/22.
//
#pragma once

#include <Box2D/Box2D.h>
#include "Missile.hpp"
#include "GameObject.hpp"

enum BulletTypes {
    None,
    Regular,
    Rocket
};

class Bullet : public Missile {
public:
    explicit Bullet(GameObject* gameObject);

    int damage = 1;
    int speed = 500;

    glm::vec2 velocity;
    glm::vec2 acceleration;

    // raycast callback
    float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction){
        return Missile::ReportFixture(fixture, point, normal, fraction);
    };
};

class RegularBullet : public Bullet{
public:
    RegularBullet(GameObject* gameObject) : Bullet(gameObject) {};
};

class RocketBullet : public Bullet {
public:

    float explosionRadius   = 8.0f;
    float explosionDamage   = 2.0f;
    float explosionForce    = 10.0f;
    float steer_force = 15.0f;

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

    //void update(float deltaTime) override; 
};
