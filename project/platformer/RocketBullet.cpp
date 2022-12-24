//
// Created by Giorgio Perri  on 31/10/22.
//

#include <Box2D/Box2D.h>
#include "PlatformerGame.hpp"
#include "Bullet.hpp"
#include "RocketBullet.hpp"
#include "Explosion.hpp"

RocketBullet::RocketBullet(GameObject* gameObject) : Bullet(gameObject) {
    
    std::cout << "Fired rocket" << std::endl;

    missilePhysics->setAutoUpdate(false);

    auto sprite = PlatformerGame::instance->getSpriteAtlas()->get("missile_0.png");
    spriteComponent->setSprite( sprite );
};

// Raycast callback
float32 RocketBullet::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    explode();
    gameObject->setConsumed(true);

    return 0;
};

void RocketBullet::onCollisionEnd(PhysicsComponent *comp) {
}
void RocketBullet::onCollisionStart(PhysicsComponent *comp) {
    Bullet::onCollisionStart(comp); 
}

void RocketBullet::explode() {

    auto go = PlatformerGame::instance->createGameObject();
    go->setPosition(gameObject->getPosition());

    auto explosion = go->addComponent<Explosion>();
}

// TODO maybe change to raycast / AABB query
    // //find all bodies with fixtures in blast radius AABB
    // MyQueryCallback queryCallback; //see "World querying topic"
    // b2AABB aabb;
    // b2Vec2 center = b2Vec2(gameObject->getPosition().x, gameObject->getPosition().y);
    // aabb.lowerBound = center - b2Vec2( explosionRadius, explosionRadius );
    // aabb.upperBound = center + b2Vec2( explosionRadius, explosionRadius );

    // PlatformerGame::instance->world->QueryAABB( &queryCallback, aabb );
    
    // //check which of these bodies have their center of mass within the blast radius
    // for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
    //     b2Body* body = queryCallback.foundBodies[i];
    //     b2Vec2 bodyCom = body->GetWorldCenter(); 
        
    //     //ignore bodies outside the blast range
    //     if ( (bodyCom - center).Length() >= explosionRadius )
    //         continue;

    //     float blastPower = 100.0f;
            
    //     b2Vec2 blastDir = bodyCom - center;
    //     float distance = blastDir.Normalize();

    //     //ignore bodies exactly at the blast point - blast direction is undefined
    //     if ( distance == 0 )
    //         return;

    //     float invDistance = 1 / distance;
    //     float impulseMag = blastPower * invDistance * invDistance;

    //     body->ApplyLinearImpulse( impulseMag * blastDir, bodyCom, true );
        
    // }


// void update(float deltaTime) override {
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
