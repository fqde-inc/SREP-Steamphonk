
#include <sre/Inspector.hpp>
#include "PlatformerGame.hpp"
#include "GameObject.hpp"
#include "sre/RenderPass.hpp"
#include "sre/Texture.hpp"
#include "SpriteComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "PhysicsComponent.hpp"
#include "CharacterController.hpp"
#include "EnemyComponent.hpp"
#include "FollowPathComponent.hpp"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include <fstream>
#include "Crosshair.hpp"
#include "Damagable.hpp"
#include "Collectible.hpp"

using namespace std;
using namespace sre;

const glm::vec2 PlatformerGame::windowSize(1400,800);
PlatformerGame* PlatformerGame::instance = nullptr;

PlatformerGame::PlatformerGame()
    :debugDraw(physicsScale)
{
    instance = this;
    r.setWindowSize(windowSize);
    bool useVsync = true;
    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
        .withSdlWindowFlags(SDL_WINDOW_OPENGL)
        .withVSync(useVsync);

    Mix_Init(0);
    currentScene = MAIN_MENU;

    backgroundColor = { 0.14f,0.12f,0.11f,1.0f };

    spriteAtlas = SpriteAtlas::create( PLATFORMER_ART_PATH + "platformer-art-deluxe.json",Texture::create()
            .withFile( PLATFORMER_ART_PATH + "platformer-art-deluxe.png")
            .withFilterSampling(false)
            .build());

    collectibleAtlas = SpriteAtlas::create( PLATFORMER_ART_PATH + "CollectibleAnim.json",Texture::create()
            .withFile( PLATFORMER_ART_PATH + "CollectibleAnim.png")
            .withFilterSampling(false)
            .build());
	
    level = Level::createDefaultLevel(this, "testlvl.json", "dirttile.json");
    level->setWorldLayer("Background");
    level->setFoliageLayer("Foliage");

    explosionAtlas = SpriteAtlas::create(EXPLOSION_ART_PATH + "explosion.json", Texture::create()
        .withFile(EXPLOSION_ART_PATH + "explosion.png")
        .withFilterSampling(false)
        .build());
	
    characterAtlas = SpriteAtlas::create( CHARACTER_ART_PATH + "characterAnims.json", Texture::create()
        .withFile( CHARACTER_ART_PATH + "characterAnims.png")
        .withFilterSampling(false)
        .build());

    initLevel();


    // setup callback functions
    r.keyEvent = [&](SDL_Event& e){
        handleInput(e);
    };
    r.frameUpdate = [&](float deltaTime){
        update(deltaTime);
    };
    r.frameRender = [&](){
        render();
    };

    r.mouseEvent = [&](SDL_Event& event) {
        mouseMotion = event.motion;
        mouseButton = event.button;
    };
	
    // start game loop
    r.startEventLoop();
}

void PlatformerGame::generateSingleBird(std::pair<int, int> coords, std::vector<glm::vec2> positions, PathType type)
{
    auto birdObj = PlatformerGame::instance->createGameObject();
    birdObj->name = "Bird";
    auto spriteComponent = birdObj->addComponent<SpriteComponent>();
    auto bird = PlatformerGame::instance->getSpriteAtlas()->get("433.png");
    bird.setFlip({ true,false });
    spriteComponent->setSprite(bird);

    auto enemy = birdObj->addComponent<EnemyComponent>();
    enemy->setPathing(positions, type);

    currentLevelBirds.push_back(birdObj);
    
    cout << "Made a bird at " << coords.first << ", " << coords.second << " with " << positions.size() << " points" << endl;
}

void PlatformerGame::destroyAllBirds() {
    for (int i = 0; i < currentLevelBirds.size(); ++i) {
        currentLevelBirds[i]->setConsumed(true);
    }
}

std::shared_ptr<Level> PlatformerGame::getLevel()
{
	return level;
}

void PlatformerGame::initLevel() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_Music* music = Mix_LoadMUS( (SOUND_PATH + "phonkLoop.wav").c_str());
    Mix_VolumeMusic(20);
    Mix_Volume(-1, 64);

    deathSFX = Mix_LoadWAV( (SOUND_PATH + "death.wav").c_str());
    explosionOneSFX = Mix_LoadWAV( (SOUND_PATH + "explosion_1.wav").c_str());
    explosionTwoSFX = Mix_LoadWAV( (SOUND_PATH + "explosion_2.wav").c_str());
    explosionThreeSFX = Mix_LoadWAV( (SOUND_PATH + "explosion_3.wav").c_str());
    handgunShootSFX = Mix_LoadWAV( (SOUND_PATH + "handgun_shoot.wav").c_str());
    rocketShootSFX = Mix_LoadWAV( (SOUND_PATH + "rocket_shoot.wav").c_str());
    hitBirdSFX = Mix_LoadWAV( (SOUND_PATH + "hit_player.wav").c_str());
    hitPlayerSFX = Mix_LoadWAV( (SOUND_PATH + "hit_bird.wav").c_str());
    hitWallSFX = Mix_LoadWAV( (SOUND_PATH + "hit_wall.wav").c_str());
    jumpSFX = Mix_LoadWAV( (SOUND_PATH + "jump.wav").c_str());
    startGameSFX = Mix_LoadWAV( (SOUND_PATH + "start_game.wav").c_str());

    Mix_PlayMusic(music, -1);

    keyboardState = SDL_GetKeyboardState(NULL);
    initPhysics();
    player = createGameObject();
    auto playerSprite = player->addComponent<SpriteComponent>();
    auto playerSpriteObj = characterAtlas->get("tile000.png");
    playerSpriteObj.setPosition(glm::vec2{1.5,2.5}*Level::tileSize);
    playerSpriteObj.setScale(glm::vec2(.7,.7));
    playerSprite->setSprite(playerSpriteObj);

    characterController = player->addComponent<CharacterController>();
    
    auto camObj = createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize * 0.5f);
    camera->setFollowObject(player, { 0,0});
    camera->setZoomMode(true);

    crosshair = createGameObject();
    crosshair->name = "Crosshair";
    auto crosshairSpriteComponent = crosshair->addComponent<SpriteComponent>();
    auto crosshairSprite = characterAtlas->get("crosshair.png");
    crosshairSprite.setScale({0.3f, 0.3f});
    crosshairSprite.setOrderInBatch(99);
    crosshairSpriteComponent->setSprite(crosshairSprite);
    crosshair->addComponent<Crosshair>();

    auto& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    pixelated = io.Fonts->AddFontFromFileTTF( (UI_ART_PATH + "PixelatedFont.ttf").c_str() , 20);
    io.FontDefault = io.Fonts->Fonts[0];

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;

    menuBG = Texture::create().withFile(UI_ART_PATH + "menu_BG.png").withFilterSampling(false).build();
    howToPlayTexture = Texture::create().withFile(UI_ART_PATH + "howToPlay.png").withFilterSampling(false).build();

    heartFull  = Texture::create().withFile( UI_ART_PATH + "heartFull.png").withFilterSampling(false).build();
    heartFullTexture = heartFull.get();

    heartEmpty = Texture::create().withFile( UI_ART_PATH + "heartEmpty.png").withFilterSampling(false).build();
    heartEmptyTexture = heartEmpty.get();

    missileUp = Texture::create().withFile( UI_ART_PATH + "missileUp.png").withFilterSampling(false).build();

    missileDown = Texture::create().withFile( UI_ART_PATH + "missileDown.png").withFilterSampling(false).build();

    handgunUp = Texture::create().withFile( UI_ART_PATH + "handgunUp.png").withFilterSampling(false).build();

    handgunDown = Texture::create().withFile( UI_ART_PATH + "handgunDown.png").withFilterSampling(false).build();

    HandgunCollectible = createGameObject();
    HandgunCollectible->position = level->getIdentifierPosition("PistolStart");

    auto pickupGunCollectible = HandgunCollectible->addComponent<Collectible>();
    pickupGunCollectible->initCollectible(Handgun);

    RocketLauncherCollectible = createGameObject();
    RocketLauncherCollectible->position = level->getIdentifierPosition("RocketStart");

    auto pickupRocketCollectible = RocketLauncherCollectible->addComponent<Collectible>();
    pickupRocketCollectible->initCollectible(RocketLauncher);
}

void PlatformerGame::update(float time) {
    if(currentScene == MAIN_MENU) {
        return;
    }

    level->generateLevelByPosition(player->getPosition());
    updatePhysics();
	if (time > 0.03) // if framerate approx 30 fps then run two physics steps
	{
		updatePhysics();
	}
    for (int i=0;i<sceneObjects.size();i++){
        if(sceneObjects[i] == nullptr) 
            continue;

        if(sceneObjects[i]->consumed){
            sceneObjects.erase(sceneObjects.begin() + i++);
            continue;
        }

        sceneObjects[i]->update(time);
    }

    if(shake)
        screenshake();

}
void PlatformerGame::setScreenshake(Shakes type){
        currentShake = type;
        float duration, value;
        switch (currentShake)
            {
                case MILD_LITTLE_PONY:
                    duration   = 0.7f;
                    value      = 10.0f;
                    break;
                
                case STEAMPHONK:
                    duration   = 1;
                    value      = 15.0f;
                    break;
                
                case EPILECTIC_DELIGHT:
                    duration   = 1.75f;
                    value      = 30.0f;
                    break;
                
                case ULTRAKILL:
                    duration   = 2;
                    value      = 40.0f;
                    break;

                case CLOVIS_FRIDAY_NIGHT:
                    duration   = 2.5f;
                    value      = 60.0f;
                    break;
            }

        if(shakeValue == 0 || value >= shakeValue) {
            shake           = true;
            shakeValue      = value;
            shakeDuration   = duration;
            shakeFade       = value / (duration * 10);

            camera->offset  = {0,0};
        }
    }

void PlatformerGame::screenshake() {

    if(shakeValue <= 0) {
        shakeValue = 0;
        shake = false;
        camera->offset = {0,0};
        return;
    }

    glm::vec2 shakeOffset = glm::linearRand(glm::vec2(-shakeValue), glm::vec2(shakeValue));

    camera->offset = shakeOffset;

    lastShake   = shakeOffset;
    shakeValue -= shakeFade;
}

void PlatformerGame::render() {

    auto rp = RenderPass::create()
            .withCamera(camera->getCamera())
            .withClearColor(true, backgroundColor)
            .build();

    ImVec2 uv0(0,1); // flip y axis coordinates
    ImVec2 uv1(1,0);
    static ImVec4 color_multipler(1, 1, 1, 0.2f);

    if(currentScene == MAIN_MENU) {

        ImGui::SetNextWindowPos(ImVec2(-25,-40), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(windowSize.x + 50, windowSize.y + 50), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("menubg", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoInputs);
        ImGui::Image(menuBG->getNativeTexturePtr(),{windowSize.x + 50, windowSize.y + 160}, uv0, uv1, color_multipler);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 141, windowSize.y / 2 - 220), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(282, 55), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushFont(pixelated);
        ImGui::Begin("title", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowFontScale(2);
        ImGui::Text("%s", "STEAMPHONK");
        ImGui::PopFont();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 141, windowSize.y / 2 - 150), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(282, 55), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushFont(pixelated);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0, 0.8f, .6f});
        ImGui::Begin("startbutton", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if(ImGui::Button("Start", {280, 55})) {
            currentScene = GAMEPLAY;
            SDL_SetWindowGrab(r.getSDLWindow(), SDL_TRUE);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
        ImGui::PopStyleColor();
        ImGui::PopFont();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 141, windowSize.y / 2 - 80), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(282, 55), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushFont(pixelated);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0, 0.8f, .6f});
        ImGui::Begin("howToButton", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if(ImGui::Button("How to play", {280, 55})) {
            currentScene = HOW_TO_PLAY;
        }
        ImGui::PopStyleColor();
        ImGui::PopFont();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 115, windowSize.y / 2 + 40), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(230, 55), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushFont(pixelated);
        ImGui::Begin("shakeAmount", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::Text("%s", "Screen shake amount:");
        ImGui::PopFont();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 141, windowSize.y / 2 + 80), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(282, 55), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushFont(pixelated);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0, 0.8f, .6f});
        ImGui::Begin("shakeSetting", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        std::string tempButtonLabel;

        switch (shakeOption) {
            case MILD_LITTLE_PONY:
                tempButtonLabel = "MILD_LITTLE_PONY";
                break;
            case STEAMPHONK:
                tempButtonLabel = "STEAMPHONK";
                break;
            case EPILECTIC_DELIGHT:
                tempButtonLabel = "EPILECTIC_DELIGHT";
                break;
            case ULTRAKILL:
                tempButtonLabel = "ULTRAKILL";
                break;
            case CLOVIS_FRIDAY_NIGHT:
                tempButtonLabel = "CLOVIS_FRIDAY_NIGHT";
                break;
        }

        const char* buttonLabel = tempButtonLabel.c_str();

        if(ImGui::Button(buttonLabel, {280, 55})) {
            switch (shakeOption) {
                case MILD_LITTLE_PONY:
                    shakeOption = STEAMPHONK;
                    break;
                case STEAMPHONK:
                    shakeOption = EPILECTIC_DELIGHT;
                    break;
                case EPILECTIC_DELIGHT:
                    shakeOption = ULTRAKILL;
                    break;
                case ULTRAKILL:
                    shakeOption = CLOVIS_FRIDAY_NIGHT;
                    break;
                case CLOVIS_FRIDAY_NIGHT:
                    shakeOption = MILD_LITTLE_PONY;
                    break;
            }
        }
        ImGui::PopStyleColor();
        ImGui::PopFont();
        ImGui::End();

        return;
    } else if (currentScene == HOW_TO_PLAY) {

        ImGui::SetNextWindowPos(ImVec2(-25,-40), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(windowSize.x + 50, windowSize.y + 50), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("menubg", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoInputs);
        ImGui::Image(menuBG->getNativeTexturePtr(),{windowSize.x + 50, windowSize.y + 160}, uv0, uv1, color_multipler);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - (730 / 2), windowSize.y / 2 - (511 / 2)), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(730, 511), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("howToPlayScreen", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::Image(howToPlayTexture->getNativeTexturePtr(),{730, 511}, uv0, uv1);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 141, windowSize.y - 100), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(282, 55), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushFont(pixelated);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0, 0.8f, .6f});
        ImGui::Begin("backButton", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if(ImGui::Button("Back", {280, 55})) {
            currentScene = MAIN_MENU;
        }
        ImGui::PopStyleColor();
        ImGui::PopFont();
        ImGui::End();

        return;
    }

    if (doDebugDraw){
        static Inspector profiler;
        profiler.update();
        profiler.gui(false);
    }

    auto pos = camera->getGameObject()->getPosition();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto & go : sceneObjects){
        go->renderSprite(spriteBatchBuilder);
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    ImGui::SetNextWindowPos(ImVec2(8, windowSize.y - 40), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 15), ImGuiSetCond_Always);
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::PushFont(pixelated);
    ImGui::Begin("weapon", nullptr,  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    if(!characterController->unlockedRocketLauncher && !characterController->unlockedHandgun) {
        ImGui::Text("Current Weapon: %s", "None");
    }
    ImGui::Text("Current Weapon: %s", characterController->equippedGun == RocketLauncher ? "Rocket Launcher" : "Handgun");
    ImGui::PopFont();
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(8, windowSize.y - 80), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 15), ImGuiSetCond_Always);
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::PushFont(pixelated);
    ImGui::Begin("Health", nullptr,  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::Text("%s", "Health:");
    ImGui::PopFont();
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(61, windowSize.y - 85), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiSetCond_Always);
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::Begin("hp", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    for (int i=0;i<characterController->damageComponent->getMaxLife();i++){
        ImGui::SameLine(40 * i + 32);
        ImGui::Image(i >= characterController->damageComponent->getCurLife() ? heartEmptyTexture->getNativeTexturePtr() : heartFullTexture->getNativeTexturePtr(),{32, 32}, uv0, uv1);
    }
    ImGui::End();

    if(characterController->unlockedHandgun) {
        ImGui::SetNextWindowPos(ImVec2(windowSize.x - 260, windowSize.y - 115), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("handgun", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        for (int i=0;i<characterController->handgun->maxClipSize;i++){
            ImGui::SameLine(40 * i + 32);
            ImGui::Image(i >= characterController->handgun->clipSize ? handgunDown->getNativeTexturePtr() : handgunUp->getNativeTexturePtr(),{48, 48}, uv0, uv1);
        }
        ImGui::End();
    }

    if(characterController->unlockedRocketLauncher){
        ImGui::SetNextWindowPos(ImVec2(windowSize.x - 95, windowSize.y - 65), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiSetCond_Always);
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("rocket", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        for (int i=0;i<characterController->rocketLauncher->maxClipSize;i++){
            ImGui::SameLine(40 * i + 32);
            ImGui::Image(i >= characterController->rocketLauncher->clipSize ? missileDown->getNativeTexturePtr() : missileUp->getNativeTexturePtr(),{48, 48}, uv0, uv1);
        }
        ImGui::End();
    }

    if (doDebugDraw){
        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();

        ImGui::SetNextWindowPos(ImVec2(0, .0f), ImGuiSetCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::Text("%s", "Current States");
        for (int i = 0; i < characterController->state_->characterStateStack.size(); ++i) {
            switch (characterController->state_->characterStateStack[i]->stateType) {
                case Standing:
                    ImGui::BulletText("%s","Standing");
                    break;
                case Jumping:
                    ImGui::BulletText("%s","Jumping");
                    break;
                case Walking:
                    ImGui::BulletText("%s","Walking");
                    break;
            }
        }
        ImGui::End();
    }
}

void PlatformerGame::handleInput(SDL_Event &event) {
    for (auto & gameObject: sceneObjects) {
        for (auto & c : gameObject->getComponents()){
            bool consumed = c->handleInput(event);
            if (consumed){
                return;
            }
        }
    }

    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
            case SDLK_z:
                camera->setZoomMode(!camera->isZoomMode());
                break;
            case SDLK_i:
                // press 'i' for physics debug
                doDebugDraw = !doDebugDraw;
                if (doDebugDraw){
                    world->SetDebugDraw(&debugDraw);
                } else {
                    world->SetDebugDraw(nullptr);
                }
                break;
        }
    }
}

std::shared_ptr<GameObject> PlatformerGame::createGameObject() {
    auto obj = shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}

void PlatformerGame::updatePhysics() {

    const int positionIterations = 4;
    const int velocityIterations = 12;
    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto phys : physicsComponentLookup){
        PhysicsComponent* physicsComponent = phys.second;
        if (physicsComponent->isAutoUpdate() == false) continue;
        auto position = physicsComponent->getBody()->GetPosition();
        float angle = physicsComponent->getBody()->GetAngle();
        auto gameObject = physicsComponent->getGameObject();
        gameObject->setPosition(glm::vec2(position.x*physicsScale, position.y*physicsScale));
        gameObject->setRotation(angle);
    }
}

void PlatformerGame::initPhysics() {
    float gravity = -9.8f; // 9.8 m/s2
    delete world;
    world = new b2World(b2Vec2(0,gravity));
    world->SetContactListener(this);

    if (doDebugDraw){
        world->SetDebugDraw(&debugDraw);
    }
}

void PlatformerGame::BeginContact(b2Contact *contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void PlatformerGame::EndContact(b2Contact *contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

void PlatformerGame::deregisterPhysicsComponent(PhysicsComponent *r) {
    auto iter = physicsComponentLookup.find(r->getFixture());
    if (iter != physicsComponentLookup.end()){
        physicsComponentLookup.erase(iter);
    } else {
        assert(false); // cannot find physics object
    }
}

void PlatformerGame::registerPhysicsComponent(PhysicsComponent *r) {
    physicsComponentLookup[r->getFixture()] = r;
}

void PlatformerGame::handleContact(b2Contact *contact, bool begin) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if (physA !=physicsComponentLookup.end() && physB != physicsComponentLookup.end()){
        auto & aComponents = physA->second->getGameObject()->getComponents();
        auto & bComponents = physB->second->getGameObject()->getComponents();
        for (auto & c : aComponents){
            //cout << c->getGameObject()->name << "\n";
            if (begin){
                c->onCollisionStart(physB->second);
            } else {
                c->onCollisionEnd(physB->second);
            }
        }
        for (auto & c : bComponents){
            if (begin){
                c->onCollisionStart(physA->second);
            } else {
                c->onCollisionEnd(physA->second);
            }
        }
    }
}
