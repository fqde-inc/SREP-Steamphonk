//
// Created by S�ren Skouv
//
#pragma once

#include "GameObject.hpp"
#include <unordered_map>

using namespace std;
using namespace sre;
using namespace glm;

class ObjectPool {
public:
    static shared_ptr<ObjectPool> createPool(shared_ptr<SpriteAtlas> tileAtlas);
    shared_ptr<GameObject> tryGetInstance(const string& key);
    void releaseAllInstances();
    void addActiveInstance(const string& key, shared_ptr<GameObject> object);
private:
    int recycled = 0;
    int spawned = 0;
	unordered_multimap<string, shared_ptr<GameObject>> _pool = {};
    unordered_multimap<string, shared_ptr<GameObject>> _used = {};
    shared_ptr<SpriteAtlas> tileAtlas;
};
