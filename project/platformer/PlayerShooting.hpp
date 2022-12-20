//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#pragma once

#include "Component.hpp"
#include <vector>
#include "glm/glm.hpp"

class PlayerShooting : public Component{
public:
    explicit PlayerShooting(GameObject *gameObject);

    void update(float deltaTime) override;

    bool handleInput(SDL_Event& event) override;

    void shootAt(glm::vec2 position);
    void shootAtCursor();

    glm::vec2 getShootDirection();

private:
    float reloadTime = 0.0f;
    float reloadTimeLimit = 2.5f;
    float shootingInterval = 0.15f;

    const int maxShots = 1;
	int shotsRemaining = maxShots;

};

