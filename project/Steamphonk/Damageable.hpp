#pragma once

#include "Component.hpp"

using namespace std;

class Damageable : public Component {
public:
    explicit Damageable(GameObject* gameObject);
    void overrideDeathAction(function<void()>);
    void overrideDamageSound(function<void()>);
    void takeDamage(int damage);
    void setMaxLife(int life);
    void setLife(int life);
    void resetLife();
    int getCurLife();
    int getMaxLife();
private:
	//Void function pointer that takes no parameters, initialize to lambda function with no parameters and body
    function<void()> deathAction = [](){};
    function<void()> damageSoundAction = [](){};
    int maxLife = 10;
    int curLife = 10;
    bool hasDeathAction = false;
    bool hasDamageSound = false;
};

