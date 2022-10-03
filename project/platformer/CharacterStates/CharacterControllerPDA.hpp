//
// Created by Giorgio Perri  on 02/10/22.
//
#pragma once

#include <iostream>
#include "../CharacterController.hpp"
#include "StandingState.hpp"

class CharacterController;

enum CharacterStateTypes {
    Standing,
    Jumping
};

class CharacterState {
public:
    CharacterState();
    explicit CharacterState(int a) {};
    virtual ~CharacterState() = default;
    virtual void handleInput(CharacterController& character);
    virtual void update(CharacterController& character);

    static std::vector<std::shared_ptr<CharacterState>> characterStateStack;

    static void pushStack(const std::shared_ptr<CharacterState>& state);
    static void popStack(CharacterStateTypes type);
    static void debugStack();

    CharacterStateTypes stateType;
};

class StandingState : public CharacterState {
public:
    explicit StandingState() : CharacterState(0) {
        stateType = Standing;
    };
    void handleInput(CharacterController& character) override;
    void update(CharacterController &character) override;
};

class JumpingState : public CharacterState {
public:
    explicit JumpingState() : CharacterState(0) {
        stateType = Jumping;
    };
    void handleInput(CharacterController& character) override;
    void update(CharacterController &character) override;
};
