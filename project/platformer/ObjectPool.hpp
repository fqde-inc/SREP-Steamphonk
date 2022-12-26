//
// Created by Giorgio Perri  on 31/10/22.
//
#pragma once

#include "GameObject.hpp"

template <typename T> 
class ObjectPool {
public:
    ObjectPool(std::shared_ptr<sre::SpriteAtlas> tileAtlas);
    std::shared_ptr<T> tryGet(const std::string& key);
    void addNew(std::string name);
private:
	std::map<std::string, std::shared_ptr<T>> _pool = {};
    std::shared_ptr<sre::SpriteAtlas> tileAtlas tileAtlas;
};
