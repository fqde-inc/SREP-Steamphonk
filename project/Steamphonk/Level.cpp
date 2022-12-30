//
// Created by Søren Skouv 😎
//

#include "Level.hpp"
#include "PlatformerGame.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <sre/Inspector.hpp>
#include "ObjectPool.hpp"


shared_ptr<Level> Level::createDefaultLevel(PlatformerGame* game, string levelName, string spritesheetName) 
{
    shared_ptr<Level> res = shared_ptr<Level>(new Level());
	
    res->game = game;
    res->tileAtlas = SpriteAtlas::create(LEVEL_ART_PATH +"dirttile.json", Texture::create()
        .withFile(LEVEL_ART_PATH + "dirttile.png")
        .withFilterSampling(false)
        .build());;
	res->tilePool = ObjectPool::createPool(res->tileAtlas);
    res->foliagePool = ObjectPool::createPool(res->tileAtlas);
	res->levelName = levelName;
	res->spritesheetName = spritesheetName;

    return res;
}

void Level::initializeNameCoordMap()
{
    ifstream tfis(LEVEL_ART_PATH + spritesheetName);
    IStreamWrapper tisw(tfis);
    Document t;
    t.ParseStream(tisw);
    auto tiles = t["frames"].GetArray();
    for (int i = 0; i < tiles.Size(); i++)
    {
        auto x = tiles[i].GetObject()["frame"].GetObject()["x"].GetInt();
        auto y = tiles[i].GetObject()["frame"].GetObject()["y"].GetInt();

        nameCoordMap[make_pair(x,y)] = tiles[i].GetObject()["filename"].GetString();
    }
}

pair<int, int> Level::srepCoordinates(int x, int y, int worldX, int worldY)
{
    return make_pair(x + worldX, -y - worldY);
}

//TODO: Instead maybe do a map based on the enum
void Level::setFoliageLayer(string identifier)
{
	foliageLayer = identifier;
}

void Level::setWorldLayer(string identifier)
{
    worldLayer = identifier;
}

string& Level::getNameByCoords(pair<int, int> coords)
{
    if (nameCoordMap.empty())
    {
        initializeNameCoordMap();
    }
    auto it = nameCoordMap.find(coords);
	return it->second;
}

int Level::getLayerIndexForLevel(string identifier, int levelNo)
{
    ifstream fis(LEVEL_ART_PATH + levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    auto level = d["levels"].GetArray()[levelNo]["layerInstances"].GetArray();
	
    for (int i = 0; i < level.Size(); i++)
    {
        string res = level[i].GetObject()["__identifier"].GetString();
        if (identifier.compare(res) == 0)
        {
            return i;
        }
    }
    return -1;
}

/// <summary>
/// Loads a level from a json file
/// </summary>
/// <param name="levelNumber"></param>
void Level::generateSpecificLevel(int levelNumber, GenerationType type)
{
    ifstream fis(LEVEL_ART_PATH + levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    int index = -1;
    switch (type)
    {
        case GenerationType::World:
            cout << "Generating world" << endl;
            index = getLayerIndexForLevel(worldLayer, levelNumber);
            break;
        case GenerationType::Foliage:
            cout << "Generating foliage" << endl;
            index = getLayerIndexForLevel(foliageLayer, levelNumber);
            break;
    }
    auto level = d["levels"].GetArray()[levelNumber]["layerInstances"].GetArray()[index]["autoLayerTiles"].GetArray();
    auto worldX = d["levels"].GetArray()[levelNumber]["worldX"].GetInt();
    auto worldY = d["levels"].GetArray()[levelNumber]["worldY"].GetInt();
	
    for (int i = 0; i < level.Size(); i++)
    {
        auto pos = level[i].GetObject()["px"].GetArray();
        auto src = level[i].GetObject()["src"].GetArray();

		int x = pos[0].GetInt();
		int y = pos[1].GetInt();

        string spriteName = getNameByCoords(make_pair(src[0].GetInt(), src[1].GetInt()));
        pair<int,int> coords = srepCoordinates(x, y, worldX, worldY);

		//Instead of adding collison on the tile, only render the sprite here, then use floodfill to generate composite colliders
        switch (type)
        {
            //TODO: these methods should pool their sprites
        case GenerationType::World:
            addTile(coords, spriteName);
            break;
        case GenerationType::Foliage:
            addSprite(coords, spriteName);
            break;
        }
    }
	
    lastGenerated = levelNumber;
}

/// <summary>
/// Only generate level if the player is within position. Also handles unloading old levels and loading new levels
/// </summary>
/// <param name="target"></param>
void Level::generateLevelByPosition(vec2 target)
{
    //This is somehow called before the player is moved to their spawn
    if (target == vec2(0, 0))
    {
        return;
    }

    auto id = getLevelIdByPosition(target);

    if (lastGenerated == id || id == -1)
    {
        return;
    }
	
    //Pools all tiles for later use
    tilePool->releaseAllInstances();
    foliagePool->releaseAllInstances();

    generateSpecificLevel(id, World);
    generateSpecificLevel(id, Foliage);
    PlatformerGame::instance->destroyAllBirds();
    generateBirdsForLevel(id);
}

void Level::generateBirdsForLevel(int id)
{
    ifstream fis(LEVEL_ART_PATH + levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    auto levels = d["levels"].GetArray();

    auto entities = d["levels"].GetArray()[id]["layerInstances"].GetArray();
    auto worldX = d["levels"].GetArray()[id]["worldX"].GetInt();
    auto worldY = d["levels"].GetArray()[id]["worldY"].GetInt();
    auto uid = d["levels"].GetArray()[id]["uid"].GetInt();
    auto gSize = d["defaultGridSize"].GetInt();

    string identifier = "PatrolStart";

    for (int j = 0; j < entities.Size(); j++)
    {
        auto entity = entities[j]["entityInstances"].GetArray();

        for (int i = 0; i < entity.Size(); i++)
        {
            string compare = entity[i].GetObject()["__identifier"].GetString();
            if (identifier.compare(compare) == 0)
            {
                auto pos = entity[i].GetObject()["px"].GetArray();
                int x = pos[0].GetInt();
                int y = pos[1].GetInt();
                auto coords = srepCoordinates(x, y, worldX, worldY);

                auto path = entity[i].GetObject()["fieldInstances"].GetArray()[0].GetObject()["__value"].GetArray();
                vector<vec2> positions;
				for (int i = 0; i < path.Size(); i++)
				{
					auto p = path[i].GetObject();
                    int pgx = p["cx"].GetInt();
					int pgy = p["cy"].GetInt();
                    auto srepPos = srepCoordinates(pgx * gSize, pgy * gSize, worldX, worldY);
					positions.push_back(vec2(srepPos.first, srepPos.second));
				}

                PlatformerGame::instance->generateSingleBird(coords, positions, LINEAR);
            }
        }
    }
}

/// <summary>
/// Returns the chunk id that the player is inside
/// </summary>
/// <param name="pos"></param>
int Level::getLevelIdByPosition(vec2 pos)
{
    if (levelBounds.empty())
    {
        generateLevelBounds();
    }
	
    for (int i = 0; i < levelBounds.size(); i++)
    {
        auto b = levelBounds[i];
        if (pos.x >= b.x && pos.x <= b.xMax && pos.y <= b.y && pos.y >= b.yMax)
        {
            return i;
        }
    }

    cout << "No bounds found matching position : (" << pos.x << ", " << pos.y << "), returning default index 0" << endl;
    return 0;
}

/// <summary>
/// Caches the level bounds
/// </summary>
void Level::generateLevelBounds()
{
    ifstream fis(LEVEL_ART_PATH + levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    auto levels = d["levels"].GetArray();

    for (int i = 0; i < levels.Size(); i++)
    {
        auto h = d["levels"].GetArray()[i]["pxHei"].GetInt();
        auto w = d["levels"].GetArray()[i]["pxWid"].GetInt();
        auto x = d["levels"].GetArray()[i]["worldX"].GetInt();
        auto y = d["levels"].GetArray()[i]["worldY"].GetInt();

        cout << "Pushing bounds for :" << i << endl << " - x: " << x << " y: " << -y << " w: " << x + w << " h: " << - y - h << " to known bounds." << endl;

        Bounds bound;
        bound.x = x;
        bound.y = -y;
        bound.xMax = x + w;
        bound.yMax = - y - h;

        levelBounds.push_back(bound);
    }
}

/// <summary>
/// Returns the position of a given identifier
/// </summary>
/// <param name="identifier"></param>
/// <returns></returns>
vec2 Level::getIdentifierPosition(string identifier)
{
    ifstream fis(LEVEL_ART_PATH + levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    auto levels = d["levels"].GetArray();

    for (int i = 0; i < levels.Size(); i++)
    {
        auto entities = d["levels"].GetArray()[i]["layerInstances"].GetArray();
        for (int j = 0; j < entities.Size(); j++)
        {
            auto entity = entities[j]["entityInstances"].GetArray();
            auto worldX = d["levels"].GetArray()[i]["worldX"].GetInt();
            auto worldY = d["levels"].GetArray()[i]["worldY"].GetInt();

            for (int i = 0; i < entity.Size(); i++)
            {
                string compare = entity[i].GetObject()["__identifier"].GetString();
                cout << i << ": " << compare << endl;
                if (identifier.compare(compare) == 0)
                {
                    auto pos = entity[i].GetObject()["px"].GetArray();
                    int x = pos[0].GetInt();
                    int y = pos[1].GetInt();
                    auto coords = srepCoordinates(x, y, worldX, worldY);
                    return vec2(coords.first, coords.second);
                }
            }
        }
    }

    return vec2(0, 0);
}

void Level::addTile(pair<int, int> coords, string name) 
{
    auto res = tilePool->tryGetInstance(name);
    if (res)
    {
        res->getComponent<PhysicsComponent>()->setPhysicsPosition(vec2(coords.first, coords.second) / game->physicsScale);
    }
    else
    {
        res = createTile(coords, name);
        tilePool->addActiveInstance(name, res);
    }
}

void Level::addSprite(pair<int, int> coords, string name) 
{
    auto res = foliagePool->tryGetInstance(name);
    if (res)
    {
        res->setPosition(vec2(coords.first, coords.second));
    }
    else
    {
        res = createSprite(coords, name);
        foliagePool->addActiveInstance(name, res);
    }
}

shared_ptr<GameObject> Level::createTile(pair<int, int> pos, string name)
{
    auto gameObject = createSprite(pos, name);
    auto size = tileAtlas->get(name).getSpriteSize();
    auto physics = gameObject->addComponent<PhysicsComponent>();
    physics->initBox(
        b2_kinematicBody,
        vec2(size) / game->physicsScale * 0.5f,
        gameObject->getPosition() / game->physicsScale,
        0);
    //Required for sprite to follow
    physics->setAutoUpdate(true);
    b2Filter filter = physics->getFixture()->GetFilterData();
    filter.categoryBits = PlatformerGame::WALLS;
    physics->getFixture()->SetFilterData(filter);
    return gameObject;
}

shared_ptr<GameObject> Level::createSprite(pair<int, int> pos, string name)
{
    auto gameObject = game->createGameObject();
    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = tileAtlas->get(name);
    float tileSize = sprite.getSpriteSize().x;
    spriteComponent->setSprite(sprite);
    gameObject->name = name;
    gameObject->setPosition(vec2(pos.first, pos.second));
    return gameObject;
}