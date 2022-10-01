//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include <iostream>
#include <cmath>
#include "BirdMovementComponent.hpp"
#include "GameObject.hpp"

BirdMovementComponent::BirdMovementComponent(GameObject *gameObject) : Component(gameObject) {}

void BirdMovementComponent::update(float deltaTime) {
    time += deltaTime;
    gameObject->setPosition(computePositionAtTime(time));
}

glm::vec2 BirdMovementComponent::computePositionAtTime(float time) {
    int segment = 4 * (int) fmod( time, getNumberOfSegments());
    float t = fmod(time,1.0f);

    // todo use Quadratic Bézier spline instead
    //return glm::mix(positions[segment],positions[segment+1],t);
    return getBezierPosition(
        positions[segment],
        positions[segment+1],
        positions[segment+2],
        positions[segment+3],
        t);
}

// p(t) = (1-t)^3 * p0 + 3(1-t)^2 * p1 + 3t^2(1-t)p2 + t^3 p3

glm::vec2 BirdMovementComponent::getBezierPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t) {
    return 
        ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) * p0 ) + 
        ( 3 * ( 1 - t ) * ( 1 - t ) * t * p1 )+ 
        ( 3 * ( 1 - t ) * t * t * p2 ) + 
        ( t * t * t * p3 );
}

const std::vector<glm::vec2> &BirdMovementComponent::getPositions() {
    return positions;
}

void BirdMovementComponent::setPositions(std::vector<glm::vec2> positions) {
    this->positions = std::move(positions);
}

int BirdMovementComponent::getNumberOfSegments() {
    
    // todo return number of Quadratic Bézier spline segments instead
    return (int) (positions.size() - 1) / 4;
}

