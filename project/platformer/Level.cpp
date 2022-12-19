//
// Created by Morten Nobel-Jørgensen on 11/3/17.
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

std::shared_ptr<Level> Level::createDefaultLevel(PlatformerGame* game, std::shared_ptr<sre::SpriteAtlas> spriteAtlas, std::shared_ptr<sre::SpriteAtlas> tileAtlas) {
    std::shared_ptr<Level> res = std::shared_ptr<Level>(new Level());

    // todo initialize
    res->game = game;
    res->spriteAtlas = spriteAtlas;
    res->tileAtlas = tileAtlas;

    return res;
}

void Level::generateLevelFromFile(int levelNumber)
{
    ifstream fis("1.json");
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

        addTile(worldX + x, levelHeight - worldY - y, spriteName);
    }
}

void Level::generateLevel() {
    ifstream fis("1.json");
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

std::string Level::getNameByCoords(std::pair<int, int> coords)
{
    ifstream tfis("steamphonk.json");
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

glm::vec2 Level::getIdentifierPosition(std::string identifier)
{
    ifstream fis("1.json");
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
