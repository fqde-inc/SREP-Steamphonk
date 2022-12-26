//
// Created by Søren Skouv
//

#include "PlatformerGame.hpp"
#include "ObjectPool.hpp"

ObjectPool::ObjectPool(std::shared_ptr<sre::SpriteAtlas> tileAtlas)
{
	this->tileAtlas = tileAtlas;
}

template <typename T>
std::shared_ptr<T> ObjectPool::tryGet(const std::string& key)
{
    auto it = _pool.find(key);
    if (it == _pool.end()) 
    {
        return std::shared_ptr<T>();
    }
    auto res = it->second;
    _pool.erase(it);
    return res;
}

void ObjectPool::addNew(std::string name){
    auto gameObject = PlatformerGame::instance->createGameObject();
    gameObject->name = "Platform";
    auto res = gameObject->addComponent<PlatformComponent>();
    res->initTile(tileAtlas, coords, name);

    _pool[name] = res;
}
