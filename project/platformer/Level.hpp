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
    static std::shared_ptr<Level> createDefaultLevel(PlatformerGame* game, std::shared_ptr<sre::SpriteAtlas> spriteAtlas);

    void generateLevel();
    void generateLevelFromFile();
	
    std::shared_ptr<PlatformComponent> addPlatform(int x, int y, int startSpriteId, int length, bool kinematic);
    std::shared_ptr<PlatformComponent> addWall(int x, int y, int startSpriteId, int height);
    std::shared_ptr<PlatformComponent> addTile(int x, int y, int spriteId);

    static constexpr float tileSize = 21;
private:
    Level() = default;
    PlatformerGame* game;
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas;
    std::map<std::pair<int,int>, int> ldtkMap{
        {std::make_pair(0,0), 2},
        {std::make_pair(0,16), 8},
        {std::make_pair(16,0), 3}
    };
};

