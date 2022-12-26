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

enum GenerationType {
	World,
	Foliage
};

class Level {
public:
    static std::shared_ptr<Level> Level::createDefaultLevel(PlatformerGame* game, std::string levelName, std::string spritesheetName);

    std::string getNameByCoords(std::pair<int, int> coords);
	
    void generateLevel();
    void generateSpecificLevel(int levelNumber, GenerationType type);
    void generateLevelByPosition(glm::vec2 target);
    int getLevelIdByPosition(glm::vec2 pos);
    void setWorldLayer(std::string identifier);
    void setFoliageLayer(std::string identifier);
    int getLayerIndexForLevel(string identifier, int levelNo);
	
    glm::vec2 getIdentifierPosition(std::string identifier);
    std::shared_ptr<PlatformComponent> addPlatform(int x, int y, int startSpriteId, int length, bool kinematic);
    std::shared_ptr<PlatformComponent> addWall(int x, int y, int startSpriteId, int height);
    std::shared_ptr<PlatformComponent> addTile(int x, int y, std::string name);
    void addSprite(int x, int y, std::string name);

    static constexpr float tileSize = 21;
private:
    void generateLevelBounds();
    Level() = default;
    std::vector<glm::vec4> levelBounds;
	std::string levelName;
    std::string spritesheetName;
    std::string foliageLayer;
    std::string worldLayer;
	//-2 is unitialized, -1 not found
    int lastGenerated = -2;
    PlatformerGame* game;
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
};

