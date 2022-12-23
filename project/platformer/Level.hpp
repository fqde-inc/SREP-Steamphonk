#pragma once

#include <memory>
#include <sre/SpriteAtlas.hpp>

class PlatformerGame;
class GameObject;
class PlatformComponent;

enum class TileCollider {
    Block,
    BlockHalf,
    Incline,
    Decline
};

class Level {
public:
    static std::shared_ptr<Level> createDefaultLevel(PlatformerGame* game, std::shared_ptr<sre::SpriteAtlas> spriteAtlas, std::shared_ptr<sre::SpriteAtlas> tileAtlas);

    void generateLevel();
    void generateSpecificLevel(int levelNumber);
    void continouslyGenerateLevelByPosition(std::shared_ptr<GameObject> target);
    int getLevelIdByPosition(glm::vec2 pos);

    glm::vec2 getIdentifierPosition(std::string identifier);
    std::string getNameByCoords(std::pair<int, int> coords);
    std::shared_ptr<PlatformComponent> addPlatform(int x, int y, int startSpriteId, int length, bool kinematic);
    std::shared_ptr<PlatformComponent> addWall(int x, int y, int startSpriteId, int height);
    std::shared_ptr<PlatformComponent> addTile(int x, int y, std::string name);

    static constexpr float tileSize = 21;
private:
    Level() = default;
    PlatformerGame* game;
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
};

