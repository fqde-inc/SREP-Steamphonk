#include <iostream>
#include <stdio.h>
#include <cmath>
#include "FollowPathComponent.hpp"
#include "GameObject.hpp"
#include "PlatformerGame.hpp"
#include "Missile.hpp"
#include "SpriteComponent.hpp"

FollowPathComponent::FollowPathComponent(GameObject *gameObject) : Component(gameObject) {
    spriteRef = gameObject->getComponent<SpriteComponent>();
}

void FollowPathComponent::update(float deltaTime) {
    time += deltaTime;
    gameObject->setPosition(computePositionAtTime(time));
}

glm::vec2 FollowPathComponent::computePositionAtTime(float time) {
    
    // fmod is a "%" function that returns float remainders
    // e.g : If time = 10.5 sec : segment = 10 / t = 0.5f;
    
    int segment = (int) fmod( time, getNumberOfSegments()) * 4; 
    
    float t = fmod(time, flapTime);

    // Did we reach the last segment ? 
    if( segment == 0 && lastSegment == getNumberOfSegments() - 1 ) {
        looping = ! looping;
        
        auto sprite = gameObject->getComponent<SpriteComponent>()->getSprite();
        sprite.setFlip({!looping, false});

        gameObject->getComponent<SpriteComponent>()->setSprite(sprite);
    }

    lastSegment = segment;
    
    // If we're looping, iterate backwards through the segments
    if( looping ){
        segment = getNumberOfSegments() - 1 - segment;
        switch (type) {
            case BEZIER:
                return getBezierPosition(
                positions[segment+3],
                positions[segment+2],
                positions[segment+1],
                positions[segment],
                t);
            
            case CATMULL_ROW:
                return getCatmullPosition(
                positions[segment+3],
                positions[segment+2],
                positions[segment+1],
                positions[segment],
                t,
                0.21f);
            
            case LINEAR:
                return glm::mix(
                    positions[segment + 1], 
                    positions[segment],
                    t);
        }
    }

    switch (type) {
        case BEZIER:
            return getBezierPosition(
                positions[segment],
                positions[segment+1],
                positions[segment+2],
                positions[segment+3],
                t);
        
        case CATMULL_ROW:
            return getCatmullPosition(
                positions[segment],
                positions[segment+1],
                positions[segment+2],
                positions[segment+3],
                t,
                0.21f);
        
        case LINEAR:
            return glm::mix(
                positions[segment], 
                positions[segment+1],
                t);
    }
}

// Bezier curve math
glm::vec2 FollowPathComponent::getBezierPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t) {
    return 
        ( 1 - t ) * ( 1 - t ) * ( 1 - t ) * p0 
        + ( 3 *  t * ( 1 - t ) * ( 1 - t )) * p1 
        + ( 3 * t * t * ( 1 - t ) * p2 ) 
        + ( t * t * t * p3 );
}

// Catmull-Rom curve, uses the tension parameter to soften the curve at inflection point
// See : https://pomax.github.io/bezierinfo/#catmullconv
glm::vec2 FollowPathComponent::getCatmullPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t, float tension) {

    float s = 2 * tension;
    
    glm::vec2 dv1 = (p2-p0) / s;
    glm::vec2 dv2 = (p3-p1) / s;

    float c0 = (2 * t * t * t) - ( 3 * t * t) + 1;
    float c1 = (t * t * t) - ( 2 * t * t ) + t;
    float c2 = (3 * t * t) - (2 * t * t * t) ;
    float c3 = (t * t * t) - (t * t);

    return c0 * p1 + c1 * dv1 + c2 * p2 + c3 * dv2;
}

const std::vector<glm::vec2> &FollowPathComponent::getPositions() {
    return positions;
}

void FollowPathComponent::setPositions(std::vector<glm::vec2> positions) {
    this->positions = std::move(positions);
}

int FollowPathComponent::getNumberOfSegments() {
    switch (type) {
        case BEZIER:
            return int (positions.size() - 1) / 4;

        case CATMULL_ROW:
            return positions.size() - 3;

        case LINEAR:
            return positions.size() - 1;
    }
    return 0;
}
