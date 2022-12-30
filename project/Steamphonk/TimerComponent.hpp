#pragma once

#include "Component.hpp"

class TimerComponent : public Component {
public:
    bool isRunning = false;
    bool hasFinished = false;
    TimerComponent(GameObject *gameObject) : Component(gameObject) {}

    void initTimer(float time);

    void stopTimer();

    void update(float deltaTime) override;

private:
    float elapsedTime;
    float setTime = 0;
};

