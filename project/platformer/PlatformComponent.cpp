//
// Created by Søren Skouv and Morten Nobel-Jørgensen
//

#include <Box2D/Box2D.h>
#include <iostream>
#include <glm/ext.hpp>
#include "PlatformComponent.hpp"
#include "PlatformerGame.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

PlatformComponent::PlatformComponent(GameObject *gameObject) : Component(gameObject) {
    auto game = PlatformerGame::instance;
    physicsScale = game->physicsScale;
}

void PlatformComponent::initTile(std::shared_ptr<sre::SpriteAtlas> tileAtlas, std::pair<int,int> coords, std::string name)
{
    this->kinematic = false; // walls cannot be moved
    auto game = PlatformerGame::instance;
    pos = glm::vec2(coords.first, coords.second);

    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = tileAtlas->get(name);
    float tileSize = sprite.getSpriteSize().x;

    gameObject->setPosition(pos);

    spriteComponent->setSprite(sprite);
    physics = gameObject->addComponent<PhysicsComponent>();
    physics->initBox(kinematic ? b2_kinematicBody : b2_staticBody, glm::vec2{ tileSize,tileSize } / physicsScale * 0.5f, pos / physicsScale, 0);
    physics->setAutoUpdate(false);

    b2Filter filter = physics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::WALLS;
    filter.maskBits = PlatformerGame::ENEMY | PlatformerGame::PLAYER | PlatformerGame::MISSILE;
    physics->getFixture()->SetFilterData(filter);
}


void PlatformComponent::moveTo(glm::vec2 tilePos) {
    assert(kinematic);
    glm::vec2 offset{Level::tileSize/2,Level::tileSize/2};
    pos = offset + glm::vec2{tilePos.x*Level::tileSize,tilePos.y*Level::tileSize};
    this->pos = pos;

    gameObject->setPosition(pos);

    glm::vec2 physPos = pos;
    physPos.x += (width-1)*0.5f*Level::tileSize;
    physics->moveTo(physPos/physicsScale);

    for (int i=0;i<tiles.size();i++){
        pos.x += Level::tileSize;
        tiles[i]->setPosition(pos);
    }
}

glm::vec2 PlatformComponent::getPosition() {
    return pos;
}

void PlatformComponent::update(float deltaTime) {
    if (kinematic){
        glm::vec2 physPos = pos;
        physPos.x += (width-1)*0.5f*Level::tileSize;
        physics->moveTo(physPos/physicsScale);
    }
}
