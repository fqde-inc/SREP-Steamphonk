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
    void generateLevelFromFile(int levelNumber);

    glm::vec2 getIdentifierPosition(int levelNumber, std::string identifier);
	
    std::shared_ptr<PlatformComponent> addPlatform(int x, int y, int startSpriteId, int length, bool kinematic);
    std::shared_ptr<PlatformComponent> addWall(int x, int y, int startSpriteId, int height);
    std::shared_ptr<PlatformComponent> addTile(int x, int y, std::string name);

    static constexpr float tileSize = 21;
private:
    Level() = default;
    PlatformerGame* game;
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
    std::map<std::pair<int,int>, std::string> ldtkMap{
        {std::make_pair(0,0), "dirt.png"},
        {std::make_pair(0,16), "dirt_bot.png"},
        {std::make_pair(16,0), "dirt_top.png"}
    };
};

