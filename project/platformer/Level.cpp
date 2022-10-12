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

std::shared_ptr<Level> Level::createDefaultLevel(PlatformerGame* game, std::shared_ptr<sre::SpriteAtlas> spriteAtlas) {
    std::shared_ptr<Level> res = std::shared_ptr<Level>(new Level());

    // todo initialize
    res->game = game;
    res->spriteAtlas = spriteAtlas;

    return res;

}

void Level::generateLevelFromFile()
{
    using namespace rapidjson;
    ifstream fis("testlvl.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    auto level = d["levels"].GetArray()[0]["layerInstances"].GetArray()[0]["autoLayerTiles"].GetArray();
    string tileset = d["levels"].GetArray()[0]["layerInstances"].GetArray()[0]["__tilesetRelPath"].GetString();
    auto size = d["levels"].GetArray()[0]["layerInstances"].GetArray()[0]["__gridSize"].GetInt();
    tileset.insert(0, ".");
    //auto curSprite = Texture::create().withFile(tileset)
	//auto tileset = d["tilesets"].GetArray()[0]["relPath"].GetString();
    auto tex = Texture::create().withFile(tileset).build();
	
    for (int i = 0; i < level.Size(); i++)
    {
        auto pos = level[i].GetObject()["px"].GetArray();
        auto src = level[i].GetObject()["src"].GetArray();
		
		
        //std::shared_ptr<sre::SpriteAtlas> sprite = SpriteAtlas::createSingleSprite(Texture::create()
        //    .withFile(tileset)
        //    .build());

        //auto sprite = Sprite::Sprite(
        //        glm::ivec2(0, 0), //pos
        //        glm::ivec2(1, 1), //size
        //        glm::ivec2(src[0].GetInt(), src[0].GetInt()), //sprite pos
        //        glm::ivec2(size, size), //spriteSize
        //        glm::vec2(size / 2, size / 2), //anchor
        //        tex);

        //auto sprite = Sprite::Sprite();
		//sprite.setPosition(glm::vec2(src[0].GetInt(), src[0].GetInt()));
        
        auto sprite = SpriteAtlas::createSingleSprite(
            tex,
            "tile",
            glm::vec2(size / 2, size / 2),
			glm::vec2(src[0].GetInt(), src[0].GetInt()),
			glm::vec2(size, size)
        );

        //This results in various errors
        addTile(pos[0].GetInt() / 32, pos[1].GetInt() / 32, sprite);
    }
}

void Level::generateLevel() {
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

//std::shared_ptr<PlatformComponent> Level::addTile(int x, int y, std::shared_ptr<sre::SpriteAtlas> singleSpriteAtlas) {
//    auto gameObject = game->createGameObject();
//    gameObject->name = "Tile";
//    auto res = gameObject->addComponent<PlatformComponent>();
//    res->initWall(spriteAtlas, x, y, 0, 10);
//    return res;
//}

//it's kinda dumb to use a sprite atlas, use sprite instead. Alas not possible
std::shared_ptr<PlatformComponent> Level::addTile(int x, int y, std::shared_ptr<sre::SpriteAtlas> singleSpriteAtlas) {
    auto gameObject = game->createGameObject();
    gameObject->name = "tile";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initTile(singleSpriteAtlas, x, y);
    return res;
}
