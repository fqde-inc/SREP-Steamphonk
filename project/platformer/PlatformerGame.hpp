#ifndef PROJECT_DATA_PATHS
#define PROJECT_DATA_PATHS

#include <string>

using namespace std;
using namespace glm;
using namespace sre;

const string PROJECT_DATA_PATH = "project_data";

const string PLATFORMER_ART_PATH   = PROJECT_DATA_PATH + "/Platformer_Art/";
const string EXPLOSION_ART_PATH    = PROJECT_DATA_PATH + "/Explosion/";
const string CHARACTER_ART_PATH    = PROJECT_DATA_PATH + "/Character_Anims/";
const string LEVEL_ART_PATH        = PROJECT_DATA_PATH + "/levels/";
const string UI_ART_PATH           = PROJECT_DATA_PATH + "/UI/";
const string SOUND_PATH            = PROJECT_DATA_PATH + "/Sound/";

#endif

#define GLM_ENABLE_EXPERIMENTAL true
#include <glm/gtx/norm.hpp>
#include <cmath>
#include <cfenv>
#include <glm/gtc/random.hpp>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
#include "SideScrollingCamera.hpp"
#include "Box2DDebugDraw.hpp"
#include "Level.hpp"
#include "CharacterController.hpp"
#include "FollowPathComponent.hpp"
#include "EnemyComponent.hpp"
#include "Box2D/Box2D.h"
#include "SDL_mixer.h"

class PhysicsComponent;
class CharacterController;

class PlatformerGame : public b2ContactListener {
public:
    PlatformerGame();

    shared_ptr<CharacterController> characterController;

    shared_ptr<GameObject> createGameObject();
    static const vec2 windowSize;

    shared_ptr<Level> getLevel();

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;
    void generateSingleBird(pair<int, int> coords, vector<vec2> positions, PathType type);
    void destroyAllBirds();

    vector<shared_ptr<GameObject>> currentLevelBirds;

    static PlatformerGame* instance;
	
    SDL_MouseMotionEvent mouseMotion;
    SDL_MouseButtonEvent mouseButton;

    shared_ptr<GameObject> crosshair;
    shared_ptr<SideScrollingCamera> camera;

    static constexpr float32 timeStep = 1.0f / 60.0f;

    shared_ptr<SpriteAtlas> getSpriteAtlas();

    // Less expensive to store player pointer and get position on demand
    vec2 getPlayerPosition();

    shared_ptr<SpriteAtlas> characterAtlas;

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

    enum Scenes {
        MAIN_MENU,
        HOW_TO_PLAY,
        GAMEPLAY,
    };
    Scenes currentScene;
    
    enum Shakes {
        MILD_LITTLE_PONY,
        STEAMPHONK,
        EPILECTIC_DELIGHT,
        ULTRAKILL,
        CLOVIS_FRIDAY_NIGHT
    };

    Shakes shakeOption = STEAMPHONK;
    
    bool shake = false;
    Shakes currentShake;

    float shakeValue = 0;
    float shakeFade = 4.0f;
    vec2 lastShake {0,0};

    void setScreenshake(Shakes type);
    void screenshake();


    shared_ptr<SpriteAtlas> collectibleAtlas;
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
    SDLRenderer r;

    void initLevel();
    void initPhysics();
    void update(float time);

    void render();

    void handleInput(SDL_Event &event);

    void handleContact(b2Contact *contact, bool begin);

    shared_ptr<SpriteAtlas> spriteAtlas;
    shared_ptr<SpriteAtlas> UIAtlas;
    shared_ptr<SpriteAtlas> tileAtlas;
    shared_ptr<SpriteAtlas> explosionAtlas;

    vector<shared_ptr<GameObject>> sceneObjects;
    shared_ptr<GameObject> player;
    shared_ptr<GameObject> HandgunCollectible;
    shared_ptr<GameObject> RocketLauncherCollectible;


    shared_ptr<Texture> menuBG;
    shared_ptr<Texture> howToPlayTexture;

    shared_ptr<Texture> heartFull;
    Texture* heartFullTexture;

    shared_ptr<Texture> heartEmpty;
    Texture* heartEmptyTexture;

    shared_ptr<Texture> missileUp;

    shared_ptr<Texture> missileDown;

    shared_ptr<Texture> handgunUp;

    shared_ptr<Texture> handgunDown;

    void updatePhysics();

    shared_ptr<Level> level;

    Color backgroundColor;
    b2World * world = nullptr;

    ImFont* pixelated;

    //shared_ptr<FollowPathComponent> birdMovement;

    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    
    map<b2Fixture*,PhysicsComponent *> physicsComponentLookup;
    
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

