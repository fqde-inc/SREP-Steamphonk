//
// Created by S�ren Skouv
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

std::shared_ptr<GameObject> ObjectPool::tryGetInstance(const std::string& key)
{
    auto it = _pool.find(key);
	//If we dont have a match, return null
    if (it == _pool.end())
    {
        return nullptr;
    }
    recycled++;
	
	//Get the value to the key
    auto res = it->second;
    _pool.erase(it);
    _used.emplace_hint(_used.cend(), key, res);
    if (res->getComponent<PhysicsComponent>())
    {
        res->getComponent<PhysicsComponent>()->getBody()->SetActive(true);
    }
    return res;
}

void ObjectPool::releaseAllInstances()
{
	std::cout << "Moved " << _used.size() << " objects to pool. In this cycle " << spawned << " were spawned, " << recycled << " were recycled" << std::endl;
    for (auto it = _used.begin(); it != _used.end(); ++it)
    {
        std::shared_ptr<GameObject> res = it->second;
        if (res->getComponent<PhysicsComponent>())
        {
            res->getComponent<PhysicsComponent>()->getBody()->SetActive(false);
            res->getComponent<PhysicsComponent>()->setPhysicsPosition(glm::vec2(-500, 0));
        }
		//As sprites have no way to be disabled, we move them far away. This call does not affect physics bodies
		res->setPosition(glm::vec2(FLT_MAX, FLT_MAX));
        _pool.emplace_hint(_pool.cend(), res->name, res);
    }
}

void ObjectPool::addActiveInstance(const std::string& key, std::shared_ptr<GameObject> object)
{
    spawned++;
    _used.emplace_hint(_used.cend(), key, object);
}
