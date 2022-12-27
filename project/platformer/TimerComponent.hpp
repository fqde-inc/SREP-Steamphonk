#pragma once

#include "Component.hpp"

class TimerComponent : public Component {
public:
    bool isRunning = false;
    bool hasFinished = false;
    TimerComponent(GameObject *gameObject) : Component(gameObject) {}

    void initTimer(float time) {
        elapsedTime = 0;
        setTime = time;
        hasFinished = false;
        isRunning = true;
    }

    void stopTimer() {
        setTime = 0;
        isRunning = false;
    }

    void update(float deltaTime) override {
        if (setTime == 0) return;

        elapsedTime += deltaTime;

        if(elapsedTime >= setTime) {
            stopTimer();
            hasFinished = true;
        }
    }

private:
    float elapsedTime;
    float setTime = 0;
};

