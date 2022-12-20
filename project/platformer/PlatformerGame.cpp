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

    using namespace rapidjson;
    ifstream fis("testlvl.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    auto hexValue = d["bgColor"].GetString();

    backgroundColor = {0.14f,0.12f,0.11f,1.0f};

    spriteAtlas = SpriteAtlas::create("platformer-art-deluxe.json",Texture::create()
            .withFile( "platformer-art-deluxe.png")
            .withFilterSampling(false)
            .build());

    tileAtlas = SpriteAtlas::create("dirtsheet.json", Texture::create()
        .withFile("dirtsheet.png")
        .withFilterSampling(false)
        .build());

    level = Level::createDefaultLevel(this, spriteAtlas, tileAtlas);

    initLevel();

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

std::shared_ptr<Level> PlatformerGame::getLevel()
{
	return level;
}

void PlatformerGame::initLevel() {
    initPhysics();

    player = createGameObject();
    auto playerSprite = player->addComponent<SpriteComponent>();
    auto playerSpriteObj = spriteAtlas->get("19.png");
    playerSpriteObj.setPosition(glm::vec2{1.5,2.5}*Level::tileSize);
    playerSprite->setSprite(playerSpriteObj);
    auto pShooting = player->addComponent<PlayerShooting>();
    characterController = player->addComponent<CharacterController>();
    characterController->playerShooting = pShooting;
    characterController->setSprites(
            spriteAtlas->get("19.png"),
            spriteAtlas->get("20.png"),
            spriteAtlas->get("21.png"),
            spriteAtlas->get("26.png"),
            spriteAtlas->get("27.png"),
            spriteAtlas->get("28.png")
    );
		
    auto camObj = createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize * 0.5f);
    camera->setFollowObject(player, { 0,0});
    camera->setZoomMode(true);

    auto birdObj = createGameObject();
    birdObj->name = "Bird";
    auto spriteComponent = birdObj->addComponent<SpriteComponent>();
    auto bird = spriteAtlas->get("433.png");
    bird.setFlip({true,false});
    spriteComponent->setSprite(bird);
    
    auto enemy = birdObj->addComponent<EnemyComponent>();
    enemy->setPathing({
        {-50,350},
        {0,300},
        {50,350},
        {100,300},
        {150,350},
        {200,300},
        {250,350},
        {300,300},
        {350,350},
        {400,300},
        {450,350},
        {500,400},
        {550,350},
        {600,300},
        {650,350},
        {700,400},
        {750,350},
        {800,300},
        {850,350},
        {900,400},
        {950,350},
        {1000,300},
        {1050,350},
        {1100,400},
        {1150,350},
        {1200,300},
        {1250,350},
    }, BEZIER);

    birdMovement = enemy->getPathing();

    level->generateLevelFromFile(0);

    crosshair = createGameObject();
    crosshair->name = "Crosshair";
    auto crosshairSprite = crosshair->addComponent<SpriteComponent>();
    crosshairSprite->setSprite(spriteAtlas->get("28.png"));
    crosshair->addComponent<Crosshair>();
}

void PlatformerGame::update(float time) {
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

        std::vector<glm::vec3> lines;
        for (int i=0;i<5000;i++){
            float t = (i/5001.0f) * birdMovement->getNumberOfSegments();
            float t1 = ((i+1)/5001.0f) * birdMovement->getNumberOfSegments();
            auto p = birdMovement->computePositionAtTime(t);
            auto p1 = birdMovement->computePositionAtTime(t1);
            lines.push_back(glm::vec3(p,0));
            lines.push_back(glm::vec3(p1,0));
        }
        rp.drawLines(lines);
    }

    auto pos = camera->getGameObject()->getPosition();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto & go : sceneObjects){
        go->renderSprite(spriteBatchBuilder);
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

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
            case SDLK_d:
                // press 'd' for physics debug
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
