//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#pragma once

#include "Component.hpp"
#include <vector>
#include "glm/glm.hpp"

class EnemyComponent : public Component{
public:
    explicit EnemyComponent(GameObject *gameObject);

    void update(float deltaTime) override;

    glm::vec2 computePositionAtTime(float time);

    int getNumberOfSegments();
private:

    void shootAtPlayer();
    void animate();
    
    glm::vec2* target;

    float keyFrameTime = 1.0f;

    float reloadTime = 0.0f;
    float reloadTimeLimit = 2.5f;
    float shootingInterval = 0.15f;

    int shots = 3;
    int shotsRemaining = 3;

    float radius;
};

