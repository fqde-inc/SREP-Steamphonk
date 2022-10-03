//
// Created by Giorgio Perri  on 02/10/22.
//

#include <memory>
#include "CharacterControllerPDA.hpp"

std::vector<std::shared_ptr<CharacterState>> CharacterState::characterStateStack;

CharacterState::CharacterState() {
    //todo add initial state
    auto a = std::make_shared<StandingState>();
    pushStack(a);
}

void CharacterState::handleInput(CharacterController &character) {

}

void CharacterState::update(CharacterController &character) {

}

void CharacterState::pushStack(const std::shared_ptr<CharacterState>& state) {
    characterStateStack.insert(characterStateStack.begin(), state);
}

void CharacterState::popStack(CharacterStateTypes type) {
    for (int i = 0; i < characterStateStack.size(); ++i) {
        if(characterStateStack[i]->stateType == type) {
            characterStateStack.erase(std::remove(characterStateStack.begin(), characterStateStack.end(), characterStateStack[i]),
                                      characterStateStack.end());
        }
    }
}

void StandingState::handleInput(CharacterController &character) {

}

void StandingState::update(CharacterController &character) {
    debugStack();
}

void JumpingState::handleInput(CharacterController &character) {
}

void JumpingState::update(CharacterController &character) {
    debugStack();
}

void CharacterState::debugStack() {
    std::string output = "Current State Stack is: ";
    for (int i = 0; i < characterStateStack.size(); ++i) {
        switch(characterStateStack[i]->stateType) {
            case Jumping:
                output += "Jumping ";
                break;
            case Standing:
                output += "Standing ";
                break;
        }
        output += " ";
    }
    std::cout << output << std::endl;
}
