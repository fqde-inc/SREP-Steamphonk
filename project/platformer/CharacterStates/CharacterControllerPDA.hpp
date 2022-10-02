//
// Created by Giorgio Perri  on 02/10/22.
//
#pragma once
#include "../CharacterController.hpp"

class CharacterController;

class CharacterState {
public:
    CharacterState();
    virtual ~CharacterState() = default;
    virtual void handleInput(CharacterController& character);
    virtual void update(CharacterController& character);

    static std::vector<std::shared_ptr<CharacterState>> characterStateStack;

    static void pushStack(const std::shared_ptr<CharacterState>& state);
    static void popStack();
};