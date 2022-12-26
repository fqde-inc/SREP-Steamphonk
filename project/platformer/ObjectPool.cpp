//
// Created by Søren Skouv
//

#include "PlatformerGame.hpp"
#include "ObjectPool.hpp"
#include "PlatformComponent.hpp"

std::shared_ptr<ObjectPool> ObjectPool::createPool(std::shared_ptr<sre::SpriteAtlas> tileAtlas)
{
    std::shared_ptr<ObjectPool> res = std::shared_ptr<ObjectPool>(new ObjectPool());
    res->tileAtlas = tileAtlas;
    return res;
}

std::shared_ptr<GameObject> ObjectPool::get(const std::string& key)
{
    auto it = _pool.find(key);
	//If we dont have a match, add new object
    if (it == _pool.end())
    {
        addNew(key);
		//Use end iterator and decrement one
        it = --_pool.end();
    }
	//Get the value to the key
    auto res = it->second;
    _pool.erase(it);
    _pool.emplace_hint(_pool.cend(), key, res);
    return res;
}

void ObjectPool::clear()
{
    for (auto it = _used.begin(); it != _used.end(); ++it)
    {
        std::shared_ptr<GameObject> res = it->second;
        res->setPosition(glm::vec2(FLT_MAX, FLT_MAX));
        _pool.emplace_hint(_pool.cend(), res->name, res);
    }
}

void ObjectPool::addNew(const std::string& name)
{
    auto gameObject = PlatformerGame::instance->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initTile(tileAtlas, std::make_pair(0,0), name);
    _pool.emplace_hint(_pool.cend(), name, gameObject);
}
