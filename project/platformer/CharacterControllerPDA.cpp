//
// Created by Giorgio Perri  on 02/10/22.
//

#include <memory>
#include "CharacterControllerPDA.hpp"
#include <SDL_events.h>
#include "PhysicsComponent.hpp"

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
        character.characterPhysics->addImpulse({0,0.15f});
        character.characterPhysics->setLinearVelocity(glm::vec2(character.characterPhysics->getLinearVelocity().x,0));
        character.characterPhysics->addImpulse({0,0.15f});
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

void CharacterState::fire(CharacterController &character, SDL_Event &event) {
    if (event.type == SDL_KEYUP) {
        character.firing = false;
        return;
    }

    if (!character.firing) {
        character.firing = true;
        pushStack(std::make_shared<FiringState>());
    }
}

void CharacterState::swapWeapons(CharacterController &character, SDL_Event &event) {
    if (event.type == SDL_KEYUP) {
        character.swappingGun = false;
        return;
    }

    if(!character.swappingGun){
        character.swappingGun = true;
        std::tuple<std::shared_ptr<Gun>, std::shared_ptr<Gun>> swappedGuns =
                {std::get<1>(character.equippedGuns), std::get<0>(character.equippedGuns)};
        character.equippedGuns.swap(swappedGuns);
    }
}

#pragma endregion

#pragma region StandingState Methods

void StandingState::handleInput(CharacterController& character, SDL_Event &event) {
    switch (event.key.keysym.sym){
        case SDLK_SPACE:
            jump(character, event);
            break;

        case SDLK_LEFT:
            moveLeft(character, event);
            pushStack(std::make_shared<WalkingState>());
            break;

        case SDLK_RIGHT:
            moveRight(character, event);
            pushStack(std::make_shared<WalkingState>());
            break;

        case SDLK_e:
            fire(character, event);
            break;

        case SDLK_q:
            swapWeapons(character, event);
            break;
    }
}

void StandingState::update(CharacterController &character) {
}

#pragma endregion

#pragma region JumpingState Methods

void JumpingState::handleInput(CharacterController& character, SDL_Event &event) {
    switch (event.key.keysym.sym){
        case SDLK_LEFT:
            moveLeft(character, event);
            break;

        case SDLK_RIGHT:
            moveRight(character, event);
            break;

        case SDLK_e:
            fire(character, event);
            break;

        case SDLK_q:
            swapWeapons(character, event);
            break;
    }
}

void JumpingState::update(CharacterController &character) {
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

        case SDLK_LEFT:
            moveLeft(character, event);
            break;

        case SDLK_RIGHT:
            moveRight(character, event);
            break;

        case SDLK_e:
            fire(character, event);
            break;

        case SDLK_q:
            swapWeapons(character, event);
            break;
    }
}

void WalkingState::update(CharacterController &character) {
}

#pragma endregion

#pragma region FiringState Methods

void FiringState::update(CharacterController &character) {
    std::get<0>(character.equippedGuns)->Fire();
    popStack(Firing);
}

#pragma endregion
