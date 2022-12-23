//
// Created by Giorgio Perri  on 02/10/22.
//

#include <memory>
#include "CharacterControllerPDA.hpp"
#include <SDL_events.h>
#include "PhysicsComponent.hpp"
#include "PlatformerGame.hpp"

std::vector<std::shared_ptr<CharacterState>> CharacterState::characterStateStack;

#pragma region CharacterState Methods

CharacterState::CharacterState() {
    auto initState = std::make_shared<StandingState>();
    pushStack(initState);
}

void CharacterState::enter() {

}

void CharacterState::exit() {

}

void CharacterState::handleInput(CharacterController& character, SDL_Event &event) {
    characterStateStack[0]->handleInput(character, event);
}

void CharacterState::update(CharacterController &character) {

}

void CharacterState::pushStack(const std::shared_ptr<CharacterState>& state) {
    characterStateStack.insert(characterStateStack.begin(), state);
}

void CharacterState::popStack(CharacterStateTypes type) {
    for (int i = 0; i < characterStateStack.size(); ++i) {
        if(characterStateStack[i]->stateType == type) {
            // TODO characterStateStack[i]->exit();
            characterStateStack.erase(std::remove(characterStateStack.begin(), characterStateStack.end(), characterStateStack[i]),
                                      characterStateStack.end());
        }
    }
}

void CharacterState::jump(CharacterController &character, SDL_Event &event) {
    if (character.isGrounded && event.type == SDL_KEYDOWN){ // prevents double jump
        pushStack(std::make_shared<JumpingState>());
        character.characterPhysics->setLinearVelocity(glm::vec2(character.characterPhysics->getLinearVelocity().x,0));
        character.characterPhysics->addImpulse({0,0.11f});
    }
}

void CharacterState::moveLeft(CharacterController &character, SDL_Event &event) {
    character.left = event.type == SDL_KEYDOWN;
    if(event.type == SDL_KEYUP) popStack(Walking);
}

void CharacterState::moveRight(CharacterController &character, SDL_Event &event) {
    character.right = event.type == SDL_KEYDOWN;
    if(event.type == SDL_KEYUP) popStack(Walking);
}

void CharacterState::fire(CharacterController &character) {
    if (character.cooldownTimer->isRunning) {
        return;
    }

    character.cooldownTimer->initTimer(character.cooldownTime);
    pushStack(std::make_shared<FiringState>());
}

void CharacterState::swapWeapons(CharacterController &character, SDL_Event &event) {
    if (character.cooldownTimer->isRunning) {
        return;
    }

    character.cooldownTimer->initTimer(character.cooldownTime);
    switch (character.equippedGun) {
        case RocketLauncher:
            character.equippedGun = Shotgun;
            break;
        case Shotgun:
            character.equippedGun = RocketLauncher;
            break;
        default:
            break;
    }
}

#pragma endregion

#pragma region StandingState Methods

void StandingState::handleInput(CharacterController& character, SDL_Event &event) {
    switch (event.key.keysym.sym){
        case SDLK_SPACE:
            jump(character, event);
            break;

        case SDLK_a:
            moveLeft(character, event);
            pushStack(std::make_shared<WalkingState>());
            break;

        case SDLK_d:
            moveRight(character, event);
            pushStack(std::make_shared<WalkingState>());
            break;

        case SDLK_q:
            swapWeapons(character, event);
            break;
    }
}

void StandingState::update(CharacterController &character) {
    if(PlatformerGame::instance->mouseButton.button == SDL_BUTTON_LEFT && PlatformerGame::instance->mouseButton.type == SDL_MOUSEBUTTONDOWN) {
        fire(character);
    }
}

#pragma endregion

#pragma region JumpingState Methods

void JumpingState::handleInput(CharacterController& character, SDL_Event &event) {
    switch (event.key.keysym.sym){
        case SDLK_a:
            moveLeft(character, event);
            break;

        case SDLK_d:
            moveRight(character, event);
            break;

        case SDLK_q:
            swapWeapons(character, event);
            break;
    }
}

void JumpingState::update(CharacterController &character) {
    if(PlatformerGame::instance->mouseButton.button == SDL_BUTTON_LEFT && PlatformerGame::instance->mouseButton.type == SDL_MOUSEBUTTONDOWN) {
        fire(character);
    }
}

void JumpingState::enter() {
    popStack(Walking);
}

void JumpingState::exit() {
}

#pragma endregion

#pragma region WalkingState Methods

void WalkingState::handleInput(CharacterController& character, SDL_Event &event) {
    switch (event.key.keysym.sym){
        case SDLK_SPACE:
            jump(character, event);
            break;

        case SDLK_a:
            moveLeft(character, event);
            break;

        case SDLK_d:
            moveRight(character, event);
            break;

        case SDLK_q:
            swapWeapons(character, event);
            break;
    }
}

void WalkingState::update(CharacterController &character) {
    if(PlatformerGame::instance->mouseButton.button == SDL_BUTTON_LEFT && PlatformerGame::instance->mouseButton.type == SDL_MOUSEBUTTONDOWN) {
        fire(character);
    }
}

#pragma endregion

#pragma region FiringState Methods

void FiringState::update(CharacterController &character) {

    switch (character.equippedGun) {
        case RocketLauncher:
            character.rocketLauncher->Fire(*character.playerShooting);
            character.characterPhysics->addImpulse(-(character.playerShooting->getShootDirection() * character.rocketLauncher->RecoilMagnitude));
            break;
        case Shotgun:
            character.shotgun->Fire(*character.playerShooting);
            character.characterPhysics->addImpulse(-(character.playerShooting->getShootDirection() * character.shotgun->RecoilMagnitude));
            break;
        default:
            break;
    }

    popStack(Firing);
}

#pragma endregion
