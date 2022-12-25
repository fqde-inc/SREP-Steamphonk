#pragma once

#include "Component.hpp"

class Damagable : public Component {
public:
    explicit Damagable(GameObject* gameObject);
    void overrideDeathAction(std::function<void()>);
    void takeDamage(int damage);
    void setMaxLife(int life);
    void setLife(int life);
    void addLife(int life);
    void resetLife();
private:
	//Void function pointer that takes no parameters, initialize to lambda function with no parameters and body
    std::function<void()> deathAction = [](){};
    int maxLife = 10;
    int curLife = 10;
    bool hasDeathAction = false;
};

