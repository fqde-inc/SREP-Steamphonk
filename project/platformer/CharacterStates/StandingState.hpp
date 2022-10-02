//
// Created by Giorgio Perri  on 02/10/22.
//
#pragma once
#include "CharacterControllerPDA.hpp"
#include <iostream>

class StandingState : public CharacterState {
public:
    StandingState() = default;
    void handleInput(CharacterController& character) override;
    void update(CharacterController &character) override;
};
