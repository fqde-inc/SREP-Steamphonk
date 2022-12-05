//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include "MovingPlatformComponent.hpp"
#include "PlatformComponent.hpp"
#include "GameObject.hpp"
#include <SDL_stdinc.h>

MovingPlatformComponent::MovingPlatformComponent(GameObject *gameObject) : Component(gameObject)
{
    platformComponent = gameObject->getComponent<PlatformComponent>();
}

void MovingPlatformComponent::update(float deltaTime) {
    totalTime += deltaTime;
    float t = fmod( totalTime, 1.0f);

    //TODO replace with easing function 
    // https://github.com/nicolausYes/easing-functions/blob/master/src/easing.cpp
    if (fmod(totalTime, 2) > 1){
        platformComponent->moveTo(
            glm::mix(
                movementEnd,
                movementStart,
                //TODO add easing.cpp
                sin( 1.5707963 * t)
            ));
    } else {
        platformComponent->moveTo(
            glm::mix(
                movementStart,
                movementEnd,
                pow( 2, 6 * (t - 1) ) * abs( sin( t * M_PI * 3.5 ) )
                //sin( 1.5707963 * fmod( totalTime, 1.0f))
            ));
    }
}

void MovingPlatformComponent::setMovementStart(glm::vec2 pos) {
    movementStart = pos;
}

void MovingPlatformComponent::setMovementEnd(glm::vec2 pos) {
    movementEnd = pos;
}
