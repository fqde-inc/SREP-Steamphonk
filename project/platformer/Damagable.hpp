#pragma once

#include "Component.hpp"

class Damagable : public Component {
public:
    explicit Damagable(GameObject* gameObject);

    void takeDamage(int damage);
    void setMaxLife(int life);
    void setLife(int life);
    void addLife(int life);
    int getCurLife();
    
private:
    int maxLife = 10;
    int curLife = 10;
};

