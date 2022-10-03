//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include <iostream>
#include <cmath>
#include "BirdMovementComponent.hpp"
#include "GameObject.hpp"
#include "PlatformerGame.hpp"
#include "Laser.hpp"
#include "SpriteComponent.hpp"

//TODO refactor component to generic "pathFinder" 
BirdMovementComponent::BirdMovementComponent(GameObject *gameObject) : Component(gameObject) {
    //auto birdPhysics = gameObject->addComponent<PhysicsComponent>();
    //spriteComponent = gameObject->getComponent<SpriteComponent>();
}

void BirdMovementComponent::update(float deltaTime) {
    time += deltaTime;
    gameObject->setPosition(computePositionAtTime(time));

    if (fmod(time, 2.0f) >= 1.5 )
        shootAtPlayer();
}

glm::vec2 BirdMovementComponent::computePositionAtTime(float time) {
    
    // fmod is a "%" function that returns float remainders
    // e.g : If time = 10.5 sec : segment = 10 / t = 0.5f;
    int segment = (int) fmod( time, getNumberOfSegments()); 
    float t = fmod(time, 1.0f);

    // Did we reach the last segment ? 
    if( segment == 0 && lastSegment == getNumberOfSegments() - 1 ) {
        looping = !looping;
        
        auto sprite = gameObject->getComponent<SpriteComponent>()->getSprite();
        sprite.setFlip({!looping, false});
        gameObject->getComponent<SpriteComponent>()->setSprite(sprite);
    }

    lastSegment = segment;
    
    // If we're looping, iterate backwards through the segments
    if( looping ){
        segment = getNumberOfSegments() - 1 - segment;
        return getCatmullPosition(
            positions[segment+3],
            positions[segment+2],
            positions[segment+1],
            positions[segment],
            t,
            0.21f);
    }

    return getCatmullPosition(
        positions[segment],
        positions[segment+1],
        positions[segment+2],
        positions[segment+3],
        t,
        0.21f);

}

// Bezier curve math
glm::vec2 BirdMovementComponent::getBezierPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t) {
    return 
        ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) * p0 ) + 
        ( 3 * ( 1 - t ) * ( 1 - t ) * t * p1 )+ 
        ( 3 * ( 1 - t ) * t * t * p2 ) + 
        ( t * t * t * p3 );
}

// Catmull-Rom curve, uses the tension parameter to soften the curve at inflection point
// See : https://pomax.github.io/bezierinfo/#catmullconv
glm::vec2 BirdMovementComponent::getCatmullPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t, float tension) {

    float s = 2 * tension;
    
    glm::vec2 dv1 = (p2-p0) / s;
    glm::vec2 dv2 = (p3-p1) / s;

    float c0 = (2 * t * t * t) - ( 3 * t * t) + 1;
    float c1 = (t * t * t) - ( 2 * t * t ) + t;
    float c2 = (3 * t * t) - (2 * t * t * t) ;
    float c3 = (t * t * t) - (t * t);

    return c0 * p1 + c1 * dv1 + c2 * p2 + c3 * dv2;
}

const std::vector<glm::vec2> &BirdMovementComponent::getPositions() {
    return positions;
}

void BirdMovementComponent::setPositions(std::vector<glm::vec2> positions) {
    this->positions = std::move(positions);
}

int BirdMovementComponent::getNumberOfSegments() {
    // returns number of Quadratic Bézier spline segments instead
    return positions.size() - 3;
}

void BirdMovementComponent::shootAtPlayer(){

    auto go = PlatformerGame::instance->createGameObject();     
    go->setPosition(gameObject->getPosition());

    auto spriteComponent = go->addComponent<SpriteComponent>();
    spriteComponent->setSprite( PlatformerGame::instance->getSpriteAtlas()->get("433.png") );

    auto l = go->addComponent<Laser>();
    glm::vec2 direction = glm::normalize( PlatformerGame::instance->getPlayerPositon() - gameObject->getPosition() );
    l->setDirection(direction);
}
