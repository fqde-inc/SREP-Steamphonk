#pragma once

#ifndef PROJECT_DATA_PATHS
#define PROJECT_DATA_PATHS

#include <string>

const std::string PROJECT_DATA_PATH (PRE_PROJECT_DATA_PATH);

const std::string PLATFORMER_ART_PATH   = PROJECT_DATA_PATH + "/Platformer_Art/";
const std::string EXPLOSION_ART_PATH    = PROJECT_DATA_PATH + "/Explosion/";
const std::string CHARACTER_ART_PATH    = PROJECT_DATA_PATH + "/Character_Anims/";
const std::string LEVEL_ART_PATH        = PROJECT_DATA_PATH + "/levels/";
const std::string UI_ART_PATH           = PROJECT_DATA_PATH + "/UI/";
const std::string SOUND_PATH            = PROJECT_DATA_PATH + "/Sound/";

#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>

#include "SDL_mixer.h"
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "GameObject.hpp"
#include "Level.hpp"
#include "CharacterController.hpp"
#include "FollowPathComponent.hpp"
#include "SideScrollingCamera.hpp"
#include "Box2DDebugDraw.hpp"

class EnemyComponent;
class PhysicsComponent;

enum class GameState{
    Ready,
    Running,
    GameOver
};

enum class Scenes {
    MAIN_MENU,
    HOW_TO_PLAY,
    GAMEPLAY,
};

enum class Shakes {
    MILD_LITTLE_PONY,
    STEAMPHONK,
    EPILECTIC_DELIGHT,
    ULTRAKILL,
    CLOVIS_FRIDAY_NIGHT
};

class PlatformerGame : public b2ContactListener {
public:
    PlatformerGame();

    std::shared_ptr<CharacterController> characterController;

    std::shared_ptr<GameObject> createGameObject();
    static const glm::vec2 windowSize;

    std::shared_ptr<Level> getLevel();

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;
    void generateSingleBird(std::pair<int, int> coords, std::vector<glm::vec2> positions, PathType type);
    void destroyAllBirds();

    std::vector<std::shared_ptr<GameObject>> currentLevelBirds;

    static PlatformerGame* instance;
	
    SDL_MouseMotionEvent mouseMotion;
    SDL_MouseButtonEvent mouseButton;

    std::shared_ptr<GameObject> crosshair;
    std::shared_ptr<SideScrollingCamera> camera;

    static constexpr float32 timeStep = 1.0f / 60.0f;

    std::shared_ptr<sre::SpriteAtlas> getSpriteAtlas();

    // Less expensive to store player pointer and get position on demand
    glm::vec2 getPlayerPosition();

    std::shared_ptr<sre::SpriteAtlas> characterAtlas;

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    Mix_Chunk* deathSFX;
    Mix_Chunk* explosionOneSFX;
    Mix_Chunk* explosionTwoSFX;
    Mix_Chunk* explosionThreeSFX;
    Mix_Chunk* handgunShootSFX;
    Mix_Chunk* rocketShootSFX;
    Mix_Chunk* hitBirdSFX;
    Mix_Chunk* hitPlayerSFX;
    Mix_Chunk* hitWallSFX;
    Mix_Chunk* jumpSFX;
    Mix_Chunk* startGameSFX;

    Scenes currentScene;

    Shakes shakeOption = Shakes::STEAMPHONK;
    
    bool shake = false;
    Shakes currentShake;

    float shakeValue = 0;
    float shakeFade = 4.0f;
    glm::vec2 lastShake {0,0};

    void setScreenshake(Shakes type);
    void screenshake();


    std::shared_ptr<sre::SpriteAtlas> collectibleAtlas;
    const float physicsScale = 100;

    // Box2D / Physics
    enum CollisionLayers {
        WALLS       = 0x0002,
        PLAYER      = 0x0003,
        ENEMY       = 0x0004,
        MISSILE     = 0x0005,
        BULLET      = 0x0006,
        EXPLOSIONS  = 0x0007,
        COLLECTIBLE = 0x0008,
    };

private:
    sre::SDLRenderer r;

    void initLevel();
    void initPhysics();
    void update(float time);

    void render();

    void handleInput(SDL_Event &event);

    void handleContact(b2Contact *contact, bool begin);

    std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
    std::shared_ptr<sre::SpriteAtlas> UIAtlas;
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
    std::shared_ptr<sre::SpriteAtlas> explosionAtlas;

    std::vector<std::shared_ptr<GameObject>> sceneObjects;
    std::shared_ptr<GameObject> player;
    std::shared_ptr<GameObject> HandgunCollectible;
    std::shared_ptr<GameObject> RocketLauncherCollectible;


    std::shared_ptr<sre::Texture> menuBG;
    std::shared_ptr<sre::Texture> howToPlayTexture;

    std::shared_ptr<sre::Texture> heartFull;
    sre::Texture* heartFullTexture;

    std::shared_ptr<sre::Texture> heartEmpty;
    sre::Texture* heartEmptyTexture;

    std::shared_ptr<sre::Texture> missileUp;

    std::shared_ptr<sre::Texture> missileDown;

    std::shared_ptr<sre::Texture> handgunUp;

    std::shared_ptr<sre::Texture> handgunDown;

    void updatePhysics();

    std::shared_ptr<Level> level;

    sre::Color backgroundColor;
    b2World * world = nullptr;

    ImFont* pixelated;

    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    
    std::map<b2Fixture*,PhysicsComponent *> physicsComponentLookup;
    
    Box2DDebugDraw debugDraw;
    bool doDebugDraw = false;

    friend class PhysicsComponent;
    friend class Level;
    friend class CharacterController;
    friend class EnemyComponent;

    friend class Projectile;
    friend class Gun;
    friend class Bullet;
    friend class RocketBullet;
    friend class Explosion;
};

