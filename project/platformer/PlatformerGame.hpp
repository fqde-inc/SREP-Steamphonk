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

class PhysicsComponent;

enum class GameState{
    Ready,
    Running,
    GameOver
};

class PlatformerGame : public b2ContactListener {
public:
    PlatformerGame();

    std::shared_ptr<GameObject> createGameObject();
    static const glm::vec2 windowSize;

    std::shared_ptr<Level> getLevel();

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;

    static PlatformerGame* instance;
	
    SDL_MouseMotionEvent mouseMotion;
    SDL_MouseButtonEvent mouseButton;

    std::shared_ptr<GameObject> crosshair;
    std::shared_ptr<SideScrollingCamera> camera;

    static constexpr float32 timeStep = 1.0f / 60.0f;

    std::shared_ptr<sre::SpriteAtlas> getSpriteAtlas(){return spriteAtlas;};

    // Less expensive to store player pointer and get position on demand
    glm::vec2 getPlayerPositon(){return player->getPosition();};

    std::shared_ptr<sre::SpriteAtlas> characterAtlas;
private:
    sre::SDLRenderer r;

    void initLevel();
    void initPhysics();
    void spawnPlayer();
    void update(float time);

    void render();

    void handleInput(SDL_Event &event);

    void handleContact(b2Contact *contact, bool begin);

    std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
    std::shared_ptr<CharacterController> characterController;
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;

    std::vector<std::shared_ptr<GameObject>> sceneObjects;
    std::shared_ptr<GameObject> player;

    void updatePhysics();

    std::shared_ptr<Level> level;

    sre::Color backgroundColor;
    b2World * world = nullptr;

     std::shared_ptr<FollowPathComponent> birdMovement;

    const float physicsScale = 100;
    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    std::map<b2Fixture*,PhysicsComponent *> physicsComponentLookup;
    Box2DDebugDraw debugDraw;
    bool doDebugDraw = false;
    friend class PhysicsComponent;
    friend class Level;
    friend class CharacterController;
    friend class PlatformComponent;
    friend class EnemyComponent;

    friend class Missile;
};

