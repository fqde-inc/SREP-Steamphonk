//
// Created by Giorgio Perri  on 02/10/22.
//
#pragma once

#include <iostream>
#include "CharacterController.hpp"

class CharacterController;

enum CharacterStateTypes {
    Standing,
    Jumping
};

#pragma region CharacterState

    class CharacterState {
    public:
        explicit CharacterState();
        explicit CharacterState(int a) {};
        virtual ~CharacterState() = default;

        CharacterStateTypes stateType = Standing;

        virtual void handleInput(CharacterController& character, SDL_Event &event);
        virtual void update(CharacterController& character);

        static std::vector<std::shared_ptr<CharacterState>> characterStateStack;

        static void pushStack(const std::shared_ptr<CharacterState>& state);
        static void popStack(CharacterStateTypes type);

    private:
        virtual void enter();
        virtual void exit();
    };

#pragma endregion

#pragma region StandingState

    class StandingState : public CharacterState {
    public:
        explicit StandingState() : CharacterState(0) {
            stateType = Standing;
        };
        void handleInput(CharacterController& character, SDL_Event &event) override;
        void update(CharacterController &character) override;
    };

#pragma endregion

#pragma region JumpingState

    class JumpingState : public CharacterState {
    public:
        explicit JumpingState() : CharacterState(0) {
            stateType = Jumping;
        };
        void handleInput(CharacterController& character, SDL_Event &event) override;
        void update(CharacterController &character) override;
    };

#pragma endregion
