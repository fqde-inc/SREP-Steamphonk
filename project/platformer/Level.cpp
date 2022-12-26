//
// Created by Søren Skouv 😎
//

#include "Level.hpp"
#include "PlatformerGame.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "PlatformComponent.hpp"
#include "MovingPlatformComponent.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <sre/Inspector.hpp>
#include "ObjectPool.hpp"

using namespace rapidjson;
using namespace sre;
using namespace std;

std::shared_ptr<Level> Level::createDefaultLevel(PlatformerGame* game, std::string levelName, std::string spritesheetName) 
{
    std::shared_ptr<Level> res = std::shared_ptr<Level>(new Level());
    res->game = game;
    res->tileAtlas = SpriteAtlas::create("dirttile.json", Texture::create()
        .withFile("dirttile.png")
        .withFilterSampling(false)
        .build());;
	res->tilePool = ObjectPool(res->tileAtlas);
    res->foliagePool = ObjectPool(res->tileAtlas);
	res->levelName = levelName;
	res->spritesheetName = spritesheetName;

    return res;
}

std::pair<int, int> Level::srepCoordinates(int x, int y, int worldX, int worldY)
{
    return make_pair(x + worldX, -y - worldY);
}

void Level::setFoliageLayer(string identifier)
{
	foliageLayer = identifier;
}

void Level::setWorldLayer(string identifier)
{
    worldLayer = identifier;
}

std::string Level::getNameByCoords(std::pair<int, int> coords)
{
    ifstream tfis(spritesheetName);
    IStreamWrapper tisw(tfis);
    Document t;
    t.ParseStream(tisw);
    auto tiles = t["frames"].GetArray();

    for (int i = 0; i < tiles.Size(); i++)
    {
        auto x = tiles[i].GetObject()["frame"].GetObject()["x"].GetInt();
        auto y = tiles[i].GetObject()["frame"].GetObject()["y"].GetInt();

        if (x == coords.first && y == coords.second)
        {
            return tiles[i].GetObject()["filename"].GetString();
        }
    }

    return "";
}

int Level::getLayerIndexForLevel(string identifier, int levelNo)
{
    ifstream fis(levelName);
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
void Level::generateSpecificLevel(int levelNumber, GenerationType type = World)
{
    ifstream fis(levelName);
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
            index = getLayerIndexForLevel(worldLayer, levelNumber);
            break;
    }
	
    auto level = d["levels"].GetArray()[levelNumber]["layerInstances"].GetArray()[index]["autoLayerTiles"].GetArray();
    /*auto levelHeight = d["levels"].GetArray()[levelNumber]["pxHei"].GetInt();
    auto levelWidth = d["levels"].GetArray()[levelNumber]["pxWid"].GetInt();*/
    auto worldX = d["levels"].GetArray()[levelNumber]["worldX"].GetInt();
    auto worldY = d["levels"].GetArray()[levelNumber]["worldY"].GetInt();

    for (int i = 0; i < level.Size(); i++)
    {
        auto pos = level[i].GetObject()["px"].GetArray();
        auto src = level[i].GetObject()["src"].GetArray();

		int x = pos[0].GetInt();
		int y = pos[1].GetInt();

        string spriteName = getNameByCoords(std::make_pair(src[0].GetInt(), src[1].GetInt()));

        std::pair<int,int> coords = srepCoordinates(x, y, worldX, worldY);

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
void Level::generateLevelByPosition(glm::vec2 target)
{
    //This is somehow called before the player is moved to their spawn
    if (target == glm::vec2(0, 0))
    {
        return;
    }

    auto id = getLevelIdByPosition(target);

    if (lastGenerated == id || id == -1)
    {
        return;
    }
	
    //Pools all tiles for later use
    tilePool.clear();
    foliagePool.clear();

	cout << "Generating level: " << id  << " for position : (" << target.x << ", " << target.y << ")" << endl;
    generateSpecificLevel(id);
    generateSpecificLevel(id, Foliage);
}

/// <summary>
/// Returns the chunk id that the player is inside
/// </summary>
/// <param name="pos"></param>
int Level::getLevelIdByPosition(glm::vec2 pos)
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
    ifstream fis(levelName);
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
/// Generates all levels
/// </summary>
void Level::generateLevel() {
    ifstream fis(levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    auto levels = d["levels"].GetArray();

    for (int i = 0; i < levels.Size(); i++)
    {
        auto index = getLayerIndexForLevel(worldLayer, i);
		
        auto level = d["levels"].GetArray()[i]["layerInstances"].GetArray()[index]["autoLayerTiles"].GetArray();
        auto levelHeight = d["levels"].GetArray()[i]["pxHei"].GetInt();
        auto levelWidth = d["levels"].GetArray()[i]["pxWid"].GetInt();
        auto worldX = d["levels"].GetArray()[i]["worldX"].GetInt();
        auto worldY = d["levels"].GetArray()[i]["worldY"].GetInt();


        //This doesnt work wat
        for (int i = 0; i < level.Size(); i++)
        {
            auto pos = level[i].GetObject()["px"].GetArray();
            auto src = level[i].GetObject()["src"].GetArray();

            int x = pos[0].GetInt();
            int y = pos[1].GetInt();
			
            string spriteName = getNameByCoords(std::make_pair(src[0].GetInt(), src[1].GetInt()));
            std::pair<int,int> coords = srepCoordinates(x, y, worldX, worldY);
            addTile(coords, spriteName);
        }
    }
}

/// <summary>
/// Returns the position of a given identifier
/// </summary>
/// <param name="identifier"></param>
/// <returns></returns>
glm::vec2 Level::getIdentifierPosition(std::string identifier)
{
    ifstream fis(levelName);
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
            //auto levelHeight = d["levels"].GetArray()[i]["pxHei"].GetInt();
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
                    //return glm::vec2(worldX + x, /*levelHeight*/ - worldY - y);
                    auto coords = srepCoordinates(x, y, worldX, worldY);
                    return glm::vec2(coords.first, coords.second);
                }
            }
        }
    }

    return glm::vec2(0, 0);
}

std::shared_ptr<PlatformComponent> Level::addTile(std::pair<int, int> coords, string name) {
    //Check if there is an available tile with the correct name in the pool
    auto res = tilePool.get(name);
    return res->getComponent<PlatformComponent>();
}

void Level::addSprite(std::pair<int, int> coords, string name) {
    //cout << "(" << x << y << ") ";
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto pos = glm::vec2(coords.first, coords.second);

    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = tileAtlas->get(name);
    float tileSize = sprite.getSpriteSize().x;

    gameObject->setPosition(pos);

    spriteComponent->setSprite(sprite);
}
