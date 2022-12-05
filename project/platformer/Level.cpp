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
    using namespace rapidjson;
    ifstream fis("testlvl.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
	
    auto level = d["levels"].GetArray()[levelNumber]["layerInstances"].GetArray()[0]["autoLayerTiles"].GetArray();
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

        string spriteName = ldtkMap[std::make_pair(src[0].GetInt(), src[1].GetInt())];

        addTile(worldX + x, levelHeight - worldY - y, spriteName);
    }
}

void Level::generateLevel() {
    //Add test wall
    addWall(0, 0, 0, 0);

    // start wall
    addWall(-1, 0, 2, 10);

    // floor
    addPlatform(0, 0, 2, 100, false);
    // ceil
    addPlatform(0, 20, 2, 100, false);

    auto movingPlatform = addPlatform(10, 3, 2, 5, true);
    auto movingPlatformComponent = movingPlatform->getGameObject()->addComponent<MovingPlatformComponent>();
    movingPlatformComponent->setMovementStart({10,3});
    movingPlatformComponent->setMovementEnd({10,5});

    // add some more platforms
    addPlatform(15, 7, 2, 5, true);
    addPlatform(20, 3, 2, 5, true);
    addPlatform(25, 7, 2, 5, true);
    addPlatform(30, 10, 2, 5, true);
    addPlatform(35, 7, 2, 5, true);
    addPlatform(40, 3, 2, 5, true);

    // end wall
    addWall(100, 0, 2, 10);
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
    auto gameObject = game->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initTile(tileAtlas, x, y, name);
    return res;
}
