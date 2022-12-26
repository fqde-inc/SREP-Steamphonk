//
// Created by Søren Skouv
//
#pragma once

#include "GameObject.hpp"

class ObjectPool {
public:
    static std::shared_ptr<ObjectPool> createPool(std::shared_ptr<sre::SpriteAtlas> tileAtlas);
    std::shared_ptr<GameObject> tryGetInstance(const std::string& key);
    void releaseAllInstances();
private:
    void addActiveInstance(const std::string& key, std::shared_ptr<GameObject> object);
	std::map<std::string, std::shared_ptr<GameObject>> _pool = {};
    std::map<std::string, std::shared_ptr<GameObject>> _used = {};
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
};
