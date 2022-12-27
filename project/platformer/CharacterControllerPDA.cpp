//
// Created by Giorgio Perri  on 02/10/22.
//

#include <memory>
#include "CharacterControllerPDA.hpp"
#include <SDL_events.h>
#include "PhysicsComponent.hpp"
#include "Gun.hpp"
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

void CharacterState::update(CharacterController &character, float deltaTime) {

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
    character.lastIsLeft = true;
    if(event.type == SDL_KEYUP) popStack(Walking);
}

void CharacterState::moveRight(CharacterController &character, SDL_Event &event) {
    character.right = event.type == SDL_KEYDOWN;
    character.lastIsLeft = false;
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
            character.equippedGun = Handgun;
            break;
        case Handgun:
            character.equippedGun = RocketLauncher;
            break;
        default:
            break;
    }
}

void CharacterState::reload(CharacterController &character) {
    character.shotgunFired = false;
    character.rocketLauncherFired = false;

    character.reloadTimer->initTimer(character.reloadTime);
;}

#pragma endregion

#pragma region StandingState

StandingState::StandingState() : CharacterState(0) {
    stateType = Standing;
    animationSprites = {PlatformerGame::instance->characterAtlas->get("tile000.png"),
                        PlatformerGame::instance->characterAtlas->get("tile001.png"),
                        PlatformerGame::instance->characterAtlas->get("tile002.png"),
                        PlatformerGame::instance->characterAtlas->get("tile003.png"),
                        PlatformerGame::instance->characterAtlas->get("tile004.png"),
                        PlatformerGame::instance->characterAtlas->get("tile005.png"),
                        PlatformerGame::instance->characterAtlas->get("tile006.png"),
    };

    for (int i = 0; i < animationSprites.size(); ++i) {
        animationSprites[i].setScale(glm::vec2(.7f));
    }
}

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

void StandingState::update(CharacterController &character, float deltaTime) {
    if(PlatformerGame::instance->mouseButton.button == SDL_BUTTON_LEFT && PlatformerGame::instance->mouseButton.type == SDL_MOUSEBUTTONDOWN) {
        fire(character);
    }

    animationTime += deltaTime;

    if(animationTime >= animationFrameRate) {
        animationIndex = (animationIndex + 1) % animationSprites.size();
        animationTime = 0;
    }

    if(!character.reloadTimer->isRunning && (character.shotgunFired || character.rocketLauncherFired)) {
        reload(character);
    }

    animationSprites[animationIndex].setFlip({character.lastIsLeft, false});

    character.spriteComponent->setSprite(animationSprites[animationIndex]);
}

#pragma endregion

#pragma region JumpingState Methods

JumpingState::JumpingState() : CharacterState(0) {
    stateType = Jumping;
    enter();

    animationSpritesStart = {PlatformerGame::instance->characterAtlas->get("tile015.png"),
                             PlatformerGame::instance->characterAtlas->get("tile016.png"),
                             PlatformerGame::instance->characterAtlas->get("tile017.png"),
                             PlatformerGame::instance->characterAtlas->get("tile018.png"),
    };

    animationSpritesEnd = {PlatformerGame::instance->characterAtlas->get("tile025.png"),
                           PlatformerGame::instance->characterAtlas->get("tile026.png"),
                           PlatformerGame::instance->characterAtlas->get("tile027.png"),
                           PlatformerGame::instance->characterAtlas->get("tile028.png"),
                           PlatformerGame::instance->characterAtlas->get("tile029.png"),
                           PlatformerGame::instance->characterAtlas->get("tile030.png"),
                           PlatformerGame::instance->characterAtlas->get("tile031.png"),
    };

    for (int i = 0; i < animationSpritesStart.size(); ++i) {
        animationSpritesStart[i].setScale(glm::vec2(.7f));
    }

    for (int i = 0; i < animationSpritesEnd.size(); ++i) {
        animationSpritesEnd[i].setScale(glm::vec2(.7f));
    }
}

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

void JumpingState::update(CharacterController &character, float deltaTime) {
    if(PlatformerGame::instance->mouseButton.button == SDL_BUTTON_LEFT && PlatformerGame::instance->mouseButton.type == SDL_MOUSEBUTTONDOWN) {
        fire(character);
    }

    animationTime += deltaTime;

    isFalling = character.characterPhysics->getLinearVelocity().y < 0;

    if (isFalling) {
        spritesToRender = animationSpritesEnd;
    } else {
        spritesToRender = animationSpritesStart;
    }

    if(animationTime >= animationFrameRate) {
        animationIndex = (animationIndex + 1) % spritesToRender.size();
        animationTime = 0;
    }

    spritesToRender[animationIndex].setFlip({character.lastIsLeft, false});

    character.spriteComponent->setSprite(spritesToRender[animationIndex]);
}

void JumpingState::enter() {
    popStack(Walking);
}

void JumpingState::exit() {
}

#pragma endregion

#pragma region WalkingState Methods

WalkingState::WalkingState() : CharacterState(0) {
    stateType = Walking;

    animationSprites = {PlatformerGame::instance->characterAtlas->get("tile007.png"),
                        PlatformerGame::instance->characterAtlas->get("tile008.png"),
                        PlatformerGame::instance->characterAtlas->get("tile009.png"),
                        PlatformerGame::instance->characterAtlas->get("tile010.png"),
                        PlatformerGame::instance->characterAtlas->get("tile011.png"),
                        PlatformerGame::instance->characterAtlas->get("tile012.png"),
                        PlatformerGame::instance->characterAtlas->get("tile013.png"),
                        PlatformerGame::instance->characterAtlas->get("tile014.png"),
    };

    for (int i = 0; i < animationSprites.size(); ++i) {
        animationSprites[i].setScale(glm::vec2(.7f));
    }
}

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

void WalkingState::update(CharacterController &character, float deltaTime) {
    if(PlatformerGame::instance->mouseButton.button == SDL_BUTTON_LEFT && PlatformerGame::instance->mouseButton.type == SDL_MOUSEBUTTONDOWN) {
        fire(character);
    }

    animationTime += deltaTime;

    if(animationTime >= animationFrameRate) {
        animationIndex = (animationIndex + 1) % animationSprites.size();
        animationTime = 0;
    }

    animationSprites[animationIndex].setFlip({character.lastIsLeft, false});

    character.spriteComponent->setSprite(animationSprites[animationIndex]);
}

#pragma endregion

#pragma region FiringState Methods

void FiringState::update(CharacterController &character, float deltaTime) {

    switch (character.equippedGun) {
        case RocketLauncher:
            if(character.rocketLauncherFired) break;
            character.reloadTimer->initTimer(character.reloadTime);
            character.rocketLauncher->Fire(character.getGameObject()->getPosition(), character.playerShooting->getShootDirection());
            character.rocketLauncherFired = true;
            character.characterPhysics->setLinearVelocity({character.characterPhysics->getLinearVelocity().x, 0});
            character.characterPhysics->addImpulse(-(character.playerShooting->getShootDirection() * character.rocketLauncher->RecoilMagnitude));
            break;
        case Handgun:
            if(character.shotgunFired) break;
            character.reloadTimer->initTimer(character.reloadTime);
            character.shotgun->Fire(character.getGameObject()->getPosition(), character.playerShooting->getShootDirection());
            character.shotgunFired = true;
            character.characterPhysics->setLinearVelocity({character.characterPhysics->getLinearVelocity().x, 0});
            character.characterPhysics->addImpulse(-(character.playerShooting->getShootDirection() * character.shotgun->RecoilMagnitude));
            break;
        default:
            break;
    }

    popStack(Firing);
}

#pragma endregion
