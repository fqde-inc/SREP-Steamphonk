#pragma once

#include <memory>
#include <sre/SpriteAtlas.hpp>
#include "ObjectPool.hpp"
#include "rapidjson/document.h"

using namespace sre;
using namespace glm;
using namespace std;
using namespace rapidjson;

class PlatformerGame;
class GameObject;

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
    static shared_ptr<Level> createDefaultLevel(PlatformerGame* game, string levelName, string spritesheetName);

    string& getNameByCoords(pair<int, int> coords);

    void generateSpecificLevel(int levelNumber, GenerationType type = World);
    void generateLevelByPosition(vec2 target);
    int getLevelIdByPosition(vec2 pos);
    void generateBirdsForLevel(int id);
    void setWorldLayer(string identifier);
    void setFoliageLayer(string identifier);
    int getLayerIndexForLevel(string identifier, int levelNo);
    pair<int,int> srepCoordinates(int x, int y, int worldX, int worldY);
    vec2 getIdentifierPosition(string identifier);
    shared_ptr<GameObject> createTile(pair<int, int> pos, string name);
    void addTile(pair<int,int> pos, string name);
    shared_ptr<GameObject> createSprite(pair<int, int> pos, string name);
    void addSprite(pair<int, int> coords, string name);

    static constexpr float tileSize = 21;
private:
    void generateLevelBounds();
    void initializeNameCoordMap();
    Level() = default;
    shared_ptr<ObjectPool> tilePool;
    shared_ptr<ObjectPool> foliagePool;
    vector<Bounds> levelBounds;
	string levelName;
    string spritesheetName;
    string foliageLayer;
    string worldLayer;
	//-2 is unitialized, -1 not found
    int lastGenerated = -2;
    PlatformerGame* game;
    shared_ptr<SpriteAtlas> tileAtlas;
    map<pair<int, int>, string> nameCoordMap;
};

