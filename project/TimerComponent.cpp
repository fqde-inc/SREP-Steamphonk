#include "TimerComponent.hpp"

void TimerComponent::initTimer(float time) {
    elapsedTime = 0;
    setTime = time;
    hasFinished = false;
    isRunning = true;
}

void TimerComponent::stopTimer() {
    setTime = 0;
    isRunning = false;
}

void TimerComponent::update(float deltaTime) {
    if (setTime == 0) return;

    elapsedTime += deltaTime;

    if(elapsedTime >= setTime) {
        stopTimer();
        hasFinished = true;
    }
}
