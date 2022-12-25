//
// Created by Giorgio Perri  on 31/10/22.
//

#include <Box2D/Box2D.h>
#include "Bullet.hpp"

class RocketBullet : public Bullet {
public:

    float explosionRadius   = 8.0f;
    float explosionDamage   = 2.0f;
    float explosionForce    = 10.0f;
    float steer_force = 15.0f;

    // Intangibility timer
    // TODO ?
    float gracePeriod = .1f;
    std::shared_ptr<TimerComponent> graceTimer;

    RocketBullet(GameObject* gameObject);    
    
    // raycast callback
    virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
                                      const b2Vec2& normal, float32 fraction) override;

    void onCollisionStart(PhysicsComponent *comp) override;
    void onCollisionEnd(PhysicsComponent *comp) override;
    void explode();

    void update(float deltaTime) override; 

    // {
    //     acceleration = SeekTarget();
    //     velocity += acceleration * deltaTime;
    //     velocity = velocity.clamped(speed)
    //     rotation = velocity.angle()
    //     position += velocity * delta * 100
    // }

    // void SeekTarget() {
    //     if target:
    //         auto desired = (target.position - position).normalized() * speed
    //         steer = (desired - velocity).normalized() * steer_force
    // }
};

//subclass b2QueryCallback
// class MyQueryCallback : public b2QueryCallback {
// public:
//     std::vector<b2Body*> foundBodies;
    
//     bool ReportFixture(b2Fixture* fixture) {
//         foundBodies.push_back( fixture->GetBody() ); 
//         return true;//keep going to find all fixtures in the query area
//     }
// };
