//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#pragma once

#include "Component.hpp"
#include <vector>
#include "glm/glm.hpp"

class FollowPathComponent : public Component{
public:
    explicit FollowPathComponent(GameObject *gameObject);

    void update(float deltaTime) override;

    const std::vector<glm::vec2>& getPositions();
    void setPositions(std::vector<glm::vec2> positions);

    // Public for debugging purposes
    glm::vec2 computePositionAtTime(float time);
    int getNumberOfSegments();
    
private:

    glm::vec2 getBezierPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t);
    glm::vec2 getCatmullPosition(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t, float tension);


    std::vector<glm::vec2> positions;

    std::shared_ptr<Component> spriteRef; 

    float time = 0;
    int lastSegment = 0;
    bool looping = false;

    float flapTime = 1.0f;

    float reloadTime = 0.0f;
    float reloadTimeLimit = 2.5f;
    float shootingInterval = 0.15f;

    int shots = 3;
    int shotsRemaining = 3;

    float radius;
};
