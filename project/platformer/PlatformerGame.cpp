
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
#include "PlayerShooting.hpp"
#include "Damagable.hpp"

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

    //using namespace rapidjson;
    //ifstream fis("testlvl.json");
    //IStreamWrapper isw(fis);
    //Document d;
    //d.ParseStream(isw);
    //auto hexValue = d["bgColor"].GetString();

    backgroundColor = { 0.14f,0.12f,0.11f,1.0f };

    spriteAtlas = SpriteAtlas::create( PLATFORMER_ART_PATH + "platformer-art-deluxe.json",Texture::create()
            .withFile( PLATFORMER_ART_PATH + "platformer-art-deluxe.png")
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

    //std::vector<glm::vec2> positions = {
    //{ -50,350 },
    //{ 0,300 },
    //{ 50,350 },
    //{ 100,300 },
    //{ 150,350 },
    //{ 200,300 },
    //{ 250,350 },
    //{ 300,300 },
    //{ 350,350 },
    //{ 400,300 },
    //{ 450,350 },
    //{ 500,400 },
    //{ 550,350 },
    //{ 600,300 },
    //{ 650,350 },
    //{ 700,400 },
    //{ 750,350 },
    //{ 800,300 },
    //{ 850,350 },
    //{ 900,400 },
    //{ 950,350 },
    //{ 1000,300 },
    //{ 1050,350 },
    //{ 1100,400 },
    //{ 1150,350 },
    //{ 1200,300 },
    //{ 1250,350 },
    //};
	
    //generateSingleBird(make_pair(300, -600), positions, BEZIER);

    //Enable mouse lock
    SDL_SetWindowGrab(r.getSDLWindow(), SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

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
    cout << "Made a bird at " << coords.first << ", " << coords.second << " with " << positions.size() << " points" << endl;
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
    auto pShooting = player->addComponent<PlayerShooting>();
    characterController = player->addComponent<CharacterController>();
    characterController->playerShooting = pShooting;
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

    heartFull  = Texture::create().withFile( UI_ART_PATH + "heartFull.png").withFilterSampling(false).build();
    heartFullTexture = heartFull.get();

    heartEmpty = Texture::create().withFile( UI_ART_PATH + "heartEmpty.png").withFilterSampling(false).build();
    heartEmptyTexture = heartEmpty.get();

    missileUp = Texture::create().withFile( UI_ART_PATH + "missileUp.png").withFilterSampling(false).build();
    missileUpTexture = missileUp.get();

    missileDown = Texture::create().withFile( UI_ART_PATH + "missileDown.png").withFilterSampling(false).build();
    missileDownTexture = missileDown.get();

    handgunUp = Texture::create().withFile( UI_ART_PATH + "handgunUp.png").withFilterSampling(false).build();
    handgunUpTexture = handgunUp.get();

    handgunDown = Texture::create().withFile( UI_ART_PATH + "handgunDown.png").withFilterSampling(false).build();
    handgunDownTexture = handgunDown.get();
}

void PlatformerGame::update(float time) {
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

}

void PlatformerGame::render() {
    auto rp = RenderPass::create()
            .withCamera(camera->getCamera())
            .withClearColor(true, backgroundColor)
            .build();

    if (doDebugDraw){
        static Inspector profiler;
        profiler.update();
        profiler.gui(false);

        //std::vector<glm::vec3> lines;
        //for (int i=0;i<5000;i++){
        //    float t = (i/5001.0f) * birdMovement->getNumberOfSegments();
        //    float t1 = ((i+1)/5001.0f) * birdMovement->getNumberOfSegments();
        //    auto p = birdMovement->computePositionAtTime(t);
        //    auto p1 = birdMovement->computePositionAtTime(t1);
        //    lines.push_back(glm::vec3(p,0));
        //    lines.push_back(glm::vec3(p1,0));
        //}
        //rp.drawLines(lines);
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

    ImVec2 uv0(0,1); // flip y axis coordinates
    ImVec2 uv1(1,0);

    ImGui::SetNextWindowPos(ImVec2(-24 + 85, windowSize.y - 85), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(600, 200), ImGuiSetCond_Always);
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::PushFont(pixelated);
    ImGui::Begin("hp", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    for (int i=0;i<characterController->damageComponent->getMaxLife();i++){
        ImGui::SameLine(40 * i + 32);
        ImGui::Image(i >= characterController->damageComponent->getCurLife() ? heartEmptyTexture->getNativeTexturePtr() : heartFullTexture->getNativeTexturePtr(),{32, 32}, uv0, uv1);
    }
    ImGui::PopFont();
    ImGui::End();

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
                case Firing:
                    ImGui::BulletText("%s","Firing");
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
