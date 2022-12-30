#pragma once

#include <iostream>
#include "CharacterController.hpp"

class CharacterController;

enum CharacterStateTypes {
    Standing,
    Jumping,
    Walking,
    Firing
};

#pragma region CharacterState

class CharacterState {
public:
    explicit CharacterState();
    explicit CharacterState(int a) {};
    virtual ~CharacterState() = default;

    CharacterStateTypes stateType = Standing;
    float animationTime = 0;
    float animationFrameRate = 1.0f / 10.0f;

    virtual void handleInput(CharacterController& character, SDL_Event &event);
    virtual void update(CharacterController& character, float deltaTime);
    virtual void jump(CharacterController& character, SDL_Event &event);
    virtual void moveLeft(CharacterController& character, SDL_Event &event);
    virtual void moveRight(CharacterController& character, SDL_Event &event);
    virtual void fire(CharacterController& character);
    virtual void swapWeapons(CharacterController& character, SDL_Event &event);

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
        explicit StandingState();;
        void handleInput(CharacterController& character, SDL_Event &event) override;
        void update(CharacterController &character, float deltaTime) override;
    private:
        std::vector<sre::Sprite> animationSprites;
        int animationIndex = 0;
    };

#pragma endregion

#pragma region JumpingState

    class JumpingState : public CharacterState {
    public:
        explicit JumpingState();;
        void handleInput(CharacterController& character, SDL_Event &event) override;
        void update(CharacterController &character, float deltaTime) override;
        void enter() override;
        void exit() override;
    private:
        bool isFalling;
        std::vector<sre::Sprite> spritesToRender;
        std::vector<sre::Sprite> animationSpritesStart;
        std::vector<sre::Sprite> animationSpritesEnd;
        int animationIndex = 0;
    };

#pragma endregion

#pragma region WalkingState

class WalkingState : public CharacterState {
public:
    explicit WalkingState();;
    void handleInput(CharacterController& character, SDL_Event &event) override;
    void update(CharacterController &character, float deltaTime) override;
private:
    std::vector<sre::Sprite> animationSprites;
    int animationIndex = 0;
};

#pragma endregion
