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

#pragma endregion

#pragma region StandingState Methods

void StandingState::handleInput(CharacterController& character, SDL_Event &event) {
    switch (event.key.keysym.sym){
        case SDLK_SPACE:
            if (character.isGrounded && event.type == SDL_KEYDOWN){ // prevents double jump
                pushStack(std::make_shared<JumpingState>());
                character.characterPhysics->addImpulse({0,0.15f});
                character.characterPhysics->setLinearVelocity(glm::vec2(character.characterPhysics->getLinearVelocity().x,0));
                character.characterPhysics->addImpulse({0,0.15f});
            }
            break;

        case SDLK_LEFT:
            character.left = event.type == SDL_KEYDOWN;
            if (event.type == SDL_KEYDOWN) pushStack(std::make_shared<WalkingState>());
            break;

        case SDLK_RIGHT:
            character.right = event.type == SDL_KEYDOWN;
            if (event.type == SDL_KEYDOWN) pushStack(std::make_shared<WalkingState>());
            break;

        case SDLK_e:
            if (event.type == SDL_KEYUP) {
                character.firing = false;
                return;
            }

            if (!character.firing) {
                character.firing = true;
                pushStack(std::make_shared<FiringState>());
            }
            break;

        case SDLK_q:
            if (event.type == SDL_KEYUP) {
                character.swappingGun = false;
                return;
            }

            if(!character.swappingGun){
                character.swappingGun = true;
                std::tuple<std::shared_ptr<Gun>, std::shared_ptr<Gun>> swappedGuns =
                        {get<1>(character.equippedGuns), get<0>(character.equippedGuns)};
                character.equippedGuns.swap(swappedGuns);
            }
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
            character.left = event.type == SDL_KEYDOWN;
            if(event.type == SDL_KEYUP) {
                popStack(Walking);
            }
            break;

        case SDLK_RIGHT:
            character.right = event.type == SDL_KEYDOWN;
            if(event.type == SDL_KEYUP) {
                popStack(Walking);
            }
            break;

        case SDLK_e:
            if (event.type == SDL_KEYUP) {
                character.firing = false;
                return;
            }

            if (!character.firing) {
                character.firing = true;
                pushStack(std::make_shared<FiringState>());
            }
            break;

        case SDLK_q:
            if (event.type == SDL_KEYUP) {
                character.swappingGun = false;
                return;
            }

            if(!character.swappingGun){
                character.swappingGun = true;
                std::tuple<std::shared_ptr<Gun>, std::shared_ptr<Gun>> swappedGuns =
                        {get<1>(character.equippedGuns), get<0>(character.equippedGuns)};
                character.equippedGuns.swap(swappedGuns);
            }
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
            if (character.isGrounded && event.type == SDL_KEYDOWN){ // prevents double jump
                pushStack(std::make_shared<JumpingState>());
                character.characterPhysics->addImpulse({0,0.15f});
                character.characterPhysics->setLinearVelocity(glm::vec2(character.characterPhysics->getLinearVelocity().x,0));
                character.characterPhysics->addImpulse({0,0.15f});
            }
            break;

        case SDLK_LEFT:
            character.left = event.type == SDL_KEYDOWN;
            if(event.type == SDL_KEYUP) popStack(Walking);
            break;

        case SDLK_RIGHT:
            character.right = event.type == SDL_KEYDOWN;
            if(event.type == SDL_KEYUP) popStack(Walking);
            break;

        case SDLK_e:
            if (event.type == SDL_KEYUP) {
                character.firing = false;
                return;
            }

            if (!character.firing) {
                character.firing = true;
                pushStack(std::make_shared<FiringState>());
            }
            break;

        case SDLK_q:
            if (event.type == SDL_KEYUP) {
                character.swappingGun = false;
                return;
            }

            if(!character.swappingGun){
                character.swappingGun = true;
                std::tuple<std::shared_ptr<Gun>, std::shared_ptr<Gun>> swappedGuns =
                        {get<1>(character.equippedGuns), get<0>(character.equippedGuns)};
                character.equippedGuns.swap(swappedGuns);
            }
            break;
    }
}

void WalkingState::update(CharacterController &character) {
}

#pragma endregion

#pragma region FiringState Methods

void FiringState::update(CharacterController &character) {
    get<0>(character.equippedGuns)->Fire();
    popStack(Firing);
}

#pragma endregion

