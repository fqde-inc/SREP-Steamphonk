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

using namespace rapidjson;
using namespace sre;
using namespace std;

std::shared_ptr<Level> Level::createDefaultLevel(PlatformerGame* game, std::shared_ptr<sre::SpriteAtlas> spriteAtlas, std::shared_ptr<sre::SpriteAtlas> tileAtlas, std::string levelName, std::string spritesheetName) {
    std::shared_ptr<Level> res = std::shared_ptr<Level>(new Level());

    res->game = game;
    res->spriteAtlas = spriteAtlas;
    res->tileAtlas = tileAtlas;
	res->levelName = levelName;
	res->spritesheetName = spritesheetName;

    return res;
}

std::string Level::getNameByCoords(std::pair<int, int> coords)
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

        if (x == coords.first && y == coords.second)
        {
            return tiles[i].GetObject()["filename"].GetString();
        }
    }

    return "";
}

/// <summary>
/// Loads a level from a json file
/// </summary>
/// <param name="levelNumber"></param>
void Level::generateSpecificLevel(int levelNumber)
{
    ifstream fis(LEVEL_ART_PATH + levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
	
    auto level = d["levels"].GetArray()[levelNumber]["layerInstances"].GetArray()[1]["autoLayerTiles"].GetArray();
    auto levelHeight = d["levels"].GetArray()[levelNumber]["pxHei"].GetInt();
    auto levelWidth = d["levels"].GetArray()[levelNumber]["pxWid"].GetInt();
    auto worldX = d["levels"].GetArray()[levelNumber]["worldX"].GetInt();
    auto worldY = d["levels"].GetArray()[levelNumber]["worldY"].GetInt();

    for (int i = 0; i < level.Size(); i++)
    {
        auto pos = level[i].GetObject()["px"].GetArray();
        auto src = level[i].GetObject()["src"].GetArray();

		int x = pos[0].GetInt();
		int y = pos[1].GetInt();

        string spriteName = getNameByCoords(std::make_pair(src[0].GetInt(), src[1].GetInt()));

		//Instead of adding collison on the tile, only render the sprite here, then use floodfill to generate composite colliders
        addTile(worldX + x, levelHeight - worldY - y, spriteName);
    }
	
    lastGenerated = levelNumber;
}

/// <summary>
/// Only generate level if the player is within position. Also handles unloading old levels and loading new levels
/// </summary>
/// <param name="target"></param>
void Level::generateLevelByPosition(glm::vec2 target)
{
    auto id = getLevelIdByPosition(target);

    if (lastGenerated == id || id == -1)
    {
        return;
    }
		
	cout << "Generating level: " << id << endl;
    generateSpecificLevel(id);
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
        auto l = levelBounds[i];
        if (pos.x >= l[0] && pos.x <= l[0] + l[2] && pos.y >= l[1] && pos.y <= l[1] + l[3])
        {
            return i;
        }
    }

    // Handle warning
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

        levelBounds.push_back(glm::vec4(x, y, w, h));
    }
}

/// <summary>
/// Generates all levels
/// </summary>
void Level::generateLevel() {
    ifstream fis(LEVEL_ART_PATH + levelName);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    auto levels = d["levels"].GetArray();

    for (int i = 0; i < levels.Size(); i++)
    {
        auto level = d["levels"].GetArray()[i]["layerInstances"].GetArray()[2]["autoLayerTiles"].GetArray();
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

            addTile(worldX + x, levelHeight - worldY - y, spriteName);
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
            auto levelHeight = d["levels"].GetArray()[i]["pxHei"].GetInt();
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
                    return glm::vec2(worldX + x, levelHeight - worldY - y);
                }
            }
        }
    }

    return glm::vec2(0, 0);
}

std::shared_ptr<PlatformComponent> Level::addPlatform(int x, int y, int startSpriteId, int length, bool kinematic) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->init(spriteAtlas, x,y,startSpriteId, length, kinematic);
    return res;
}


std::shared_ptr<PlatformComponent> Level::addWall(int x, int y, int startSpriteId, int length) {
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initWall(spriteAtlas, x,y,startSpriteId, length);
    return res;
}

std::shared_ptr<PlatformComponent> Level::addTile(int x, int y, string name) {
    //cout << "(" << x << y << ") ";
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initTile(tileAtlas, x, y, name);
    return res;
}
