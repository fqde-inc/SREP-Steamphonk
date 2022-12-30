#pragma once

#include "Component.hpp"

using namespace std;
using namespace glm;

class Crosshair : public Component {
public:
    explicit Crosshair(GameObject* gameObject);
    void update(float deltaTime) override;
};