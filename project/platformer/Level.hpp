#pragma once

#include <memory>
#include <sre/SpriteAtlas.hpp>
#include "ObjectPool.hpp"

class PlatformerGame;
class GameObject;
class PlatformComponent;

enum class TileCollider {
    Block,
    BlockHalf,
    Incline,
    Decline
};

enum GenerationType {
	World,
	Foliage
};

struct Bounds {
    int x;
    int y;
    int xMax;
    int yMax;
};

class Level {
public:
    static std::shared_ptr<Level> Level::createDefaultLevel(PlatformerGame* game, std::string levelName, std::string spritesheetName);

    std::string getNameByCoords(std::pair<int, int> coords);
	
    void generateLevel();
    void generateSpecificLevel(int levelNumber, GenerationType type = World);
    void generateLevelByPosition(glm::vec2 target);
    int getLevelIdByPosition(glm::vec2 pos);
    void setWorldLayer(std::string identifier);
    void setFoliageLayer(std::string identifier);
    int getLayerIndexForLevel(std::string identifier, int levelNo);
    std::pair<int,int> srepCoordinates(int x, int y, int worldX, int worldY);
    glm::vec2 getIdentifierPosition(std::string identifier);
    std::shared_ptr<PlatformComponent> createTile(std::pair<int, int> pos, std::string name);
    void addTile(std::pair<int,int> pos, std::string name);
    std::shared_ptr<PlatformComponent> createSprite(std::pair<int, int> pos, std::string name);
    void addSprite(std::pair<int, int> coords, std::string name);

    static constexpr float tileSize = 21;
private:
    void generateLevelBounds();
    Level() = default;
    std::shared_ptr<ObjectPool> tilePool;
    std::shared_ptr<ObjectPool> foliagePool;
    std::vector<Bounds> levelBounds;
	std::string levelName;
    std::string spritesheetName;
    std::string foliageLayer;
    std::string worldLayer;
	//-2 is unitialized, -1 not found
    int lastGenerated = -2;
    PlatformerGame* game;
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
};

