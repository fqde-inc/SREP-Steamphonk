//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <cmath>
#include "Laser.hpp"
#include "GameObject.hpp"
#include "Component.hpp"

using namespace std;


Laser::Laser(GameObject *gameObject) : Component(gameObject) {}

void Laser::update(float deltaTime) {
    //TODO: Do we want self desintegration ?
    lifetime += deltaTime;
    if(lifetime >= lifespan ) {
        wasted = true;
    }

    gameObject->setPosition( gameObject->getPosition() + ( direction * constSpeed ));

}

//TODO: Add collision logic
// void Laser::onCollision(shared_ptr<GameObject> other) {
//     if(wasted) return;

//     if (Asteroid* asteroid = dynamic_cast<Asteroid*>(other.get())) {
//         wasted = true;
//         asteroid->destroy();
//         AsteroidsGame::incrementScore();
//         return;
//     }
// }