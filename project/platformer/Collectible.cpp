//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "Collectible.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include "PlatformerGame.hpp"
#include "EnemyComponent.hpp"

using namespace std;

Collectible::Collectible(GameObject *gameObject) : Component(gameObject) {
    gameObject->name = "Projectile";

    animationSprites = {PlatformerGame::instance->collectibleAtlas->get("collectible1.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible2.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible3.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible4.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible5.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible6.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible7.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible8.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible9.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible10.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible11.png"),
                        PlatformerGame::instance->collectibleAtlas->get("collectible12.png"),
    };

    for (int i = 0; i < animationSprites.size(); ++i) {
        animationSprites[i].setScale(glm::vec2(.4f));
        animationSprites[i].setOrderInBatch(999);
    }

    spriteComponent = gameObject->addComponent<SpriteComponent>();
    spriteComponent->setSprite(animationSprites[0]);
}

// Raycast callback
float32 Collectible::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    return 0;
}

void Collectible::onCollisionStart(PhysicsComponent *comp) {

    auto go = comp->getGameObject();

    if(go->name != "Player"){
        return;
    }

    if(gunType == Handgun) {
        cout << "gun\n";
        PlatformerGame::instance->characterController->unlockedHandgun = true;
    } else if (gunType == RocketLauncher) {
        cout << "rocket\n";
        PlatformerGame::instance->characterController->unlockedRocketLauncher = true;
    }

    gameObject->setConsumed(true);
}

void Collectible::update(float deltaTime) {
    animationTime += deltaTime;

    if(animationTime >= animationFrameRate) {
        animationIndex = (animationIndex + 1) % animationSprites.size();
        animationTime = 0;
    }

    spriteComponent->setSprite(animationSprites[animationIndex]);
}

void Collectible::initCollectible(GunTypes gunType) {
    this->gunType = gunType;

    physicsComponent = gameObject->addComponent<PhysicsComponent>();

    physicsComponent->initCircle(b2_staticBody,
                                 5 / PlatformerGame::instance->physicsScale,
                                 PlatformerGame::instance->getLevel()->getIdentifierPosition(gunType == Handgun ? "PistolStart" : "RocketStart") / PlatformerGame::instance->physicsScale,
                                 1);
    physicsComponent->setSensor(true);
}
