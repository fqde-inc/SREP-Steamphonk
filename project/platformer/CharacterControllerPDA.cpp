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
        Mix_PlayChannel(-1, PlatformerGame::instance->jumpSFX, 0);
        character.characterPhysics->setLinearVelocity(glm::vec2(character.characterPhysics->getLinearVelocity().x,0));
        character.characterPhysics->addImpulse({0,0.11f});
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

void CharacterState::fire(CharacterController &character) {
    switch (character.equippedGun) {
        case RocketLauncher:
            if(!character.unlockedRocketLauncher) break;
            if(character.rocketLauncher->Fire(character.getGameObject()->getPosition(), character.getShootDirection())){
                Mix_PlayChannel(-1, PlatformerGame::instance->rocketShootSFX, 0);
                character.reloadTimer->initTimer(character.reloadTime);
                character.characterPhysics->setLinearVelocity({character.characterPhysics->getLinearVelocity().x, 0});
                character.characterPhysics->addImpulse(-(character.getShootDirection() * character.rocketLauncher->RecoilMagnitude));
            }
            break;
        case Handgun:
            if(!character.unlockedHandgun) break;
            if(character.handgun->Fire(character.getGameObject()->getPosition(), character.getShootDirection())){
                Mix_PlayChannel(-1, PlatformerGame::instance->handgunShootSFX, 0);
                character.reloadTimer->initTimer(character.reloadTime);
                character.characterPhysics->setLinearVelocity({character.characterPhysics->getLinearVelocity().x, 0});
                character.characterPhysics->addImpulse(-(character.getShootDirection() * character.handgun->RecoilMagnitude));
            }
            break;
        default:
            break;
    }
}

void CharacterState::swapWeapons(CharacterController &character, SDL_Event &event) {

    if (character.swapTimer->isRunning || !character.unlockedHandgun || !character.unlockedRocketLauncher) {
        return;
    }

    character.swapTimer->initTimer(character.swapTime);
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

    //TODO:AHA! The error is animation related, this is just a quick fix
    if (spritesToRender.size() <= animationIndex)
    {
        std::cout << "An animation outside maximium allowed index was attempted, resetting to max" << std::endl;
        animationIndex = spritesToRender.size()-1;
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
//    if(!PlatformerGame::instance->state[SDL_SCANCODE_A] || !PlatformerGame::instance->state[SDL_SCANCODE_D]) {
//        popStack(Walking);
//    }

    if(PlatformerGame::instance->mouseButton.button == SDL_BUTTON_LEFT && PlatformerGame::instance->mouseButton.type == SDL_MOUSEBUTTONDOWN) {
        fire(character);
    }
    
    if(!PlatformerGame::instance->keyboardState[SDL_SCANCODE_A] && !PlatformerGame::instance->keyboardState[SDL_SCANCODE_D]) {
        character.left = false;
        character.right = false;
        popStack(Walking);
        return;
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
