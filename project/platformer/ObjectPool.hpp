//
// Created by Giorgio Perri  on 31/10/22.
//
#pragma once

#include "GameObject.hpp"

class ObjectPool {
public:
    ObjectPool(std::shared_ptr<sre::SpriteAtlas> tileAtlas);
    std::shared_ptr<GameObject> get(const std::string& key);
    void clear();
private:
    void addNew(const std::string& name);
	std::map<std::string, std::shared_ptr<GameObject>> _pool = {};
    std::map<std::string, std::shared_ptr<GameObject>> _used = {};
    std::shared_ptr<sre::SpriteAtlas> tileAtlas;
};
