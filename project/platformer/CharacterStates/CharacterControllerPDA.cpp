//
// Created by Giorgio Perri  on 02/10/22.
//

#include "CharacterControllerPDA.hpp"
#include "StandingState.hpp"

std::vector<std::shared_ptr<CharacterState>> CharacterState::characterStateStack;

CharacterState::CharacterState() {
    //todo add initial state
}

void CharacterState::handleInput(CharacterController &character) {

}

void CharacterState::update(CharacterController &character) {

}

void CharacterState::pushStack(const std::shared_ptr<CharacterState>& state) {
    CharacterState::characterStateStack.push_back(state);
}

void CharacterState::popStack() {
    //characterStateStack.erase();
}
