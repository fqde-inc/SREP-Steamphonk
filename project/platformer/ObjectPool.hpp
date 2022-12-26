//
// Created by Søren Skouv
//
#pragma once

#include "GameObject.hpp"

class ObjectPool {
public:
    static std::shared_ptr<ObjectPool> createPool(std::shared_ptr<sre::SpriteAtlas> tileAtlas);
    std::shared_ptr<GameObject> get(const std::string& key);
    void clear();
private:
    void addNew(const std::string& name);
	std::map<std::string, std::shared_ptr<GameObject>> _pool = {};
    std::map<std::string, std::shared_ptr<GameObject>> _used = {};
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
};
