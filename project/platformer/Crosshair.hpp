#pragma once

#include "Component.hpp"

class Crosshair : public Component {
public:
    explicit Crosshair(GameObject* gameObject);
    void update(float deltaTime) override;
};