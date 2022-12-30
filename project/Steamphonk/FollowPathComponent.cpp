#include <iostream>
#include <stdio.h>
#include <cmath>
#include "FollowPathComponent.hpp"
#include "GameObject.hpp"
#include "PlatformerGame.hpp"
#include "Projectile.hpp"
#include "SpriteComponent.hpp"

FollowPathComponent::FollowPathComponent(GameObject *gameObject) : Component(gameObject) {
    spriteRef = gameObject->getComponent<SpriteComponent>();
}

void FollowPathComponent::update(float deltaTime) {
    time += deltaTime;
    
    gameObject->setPosition(computePositionAtTime(time));
}

vec2 FollowPathComponent::computePositionAtTime(float time) {
    
    // fmod is a "%" function that returns float remainders
    // e.g : If time = 10.5 sec : segment = 10 / t = 0.5f;
    
    int segment = (int) fmod( time, getNumberOfSegments()); 
    
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
                positions[segment * 2 + 2],
                positions[segment * 2 + 1],
                positions[segment * 2],
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
                return mix(
                    positions[segment + 1], 
                    positions[segment],
                    t);
        }
    }

    switch (type) {
        case BEZIER:
            return getBezierPosition(
                positions[segment * 2],
                positions[segment * 2 + 1],
                positions[segment * 2 + 2],
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
            return mix(
                positions[segment], 
                positions[segment+1],
                t);
    }

    return vec2{0,0};
}

// Bezier curve math
vec2 FollowPathComponent::getBezierPosition(vec2 p0, vec2 p1, vec2 p2, float t) {

    vec2 v0 = mix(
        p0, p1, t
    );
    
    vec2 v1 = mix(
        p1, p2, t
    );

    return mix(v0, v1, t);
}

// Catmull-Rom curve, uses the tension parameter to soften the curve at inflection point
// See : https://pomax.github.io/bezierinfo/#catmullconv
vec2 FollowPathComponent::getCatmullPosition(vec2 p0, vec2 p1, vec2 p2, vec2 p3, float t, float tension) {

    float s = 2 * tension;
    
    vec2 dv1 = (p2-p0) / s;
    vec2 dv2 = (p3-p1) / s;

    float c0 = (2 * t * t * t) - ( 3 * t * t) + 1;
    float c1 = (t * t * t) - ( 2 * t * t ) + t;
    float c2 = (3 * t * t) - (2 * t * t * t) ;
    float c3 = (t * t * t) - (t * t);

    return c0 * p1 + c1 * dv1 + c2 * p2 + c3 * dv2;
}

void FollowPathComponent::setPositions(vector<vec2> positions) {
    this->positions = move(positions);
}

int FollowPathComponent::getNumberOfSegments() {
    switch (type) {
        case BEZIER:
            return positions.size() / 2 - 1;

        case CATMULL_ROW:
            return positions.size() - 3;

        case LINEAR:
            return positions.size() - 1;
    }
    return 0;
}
