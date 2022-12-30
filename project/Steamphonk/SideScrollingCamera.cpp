//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "SideScrollingCamera.hpp"
#include "PlatformerGame.hpp"

SideScrollingCamera::SideScrollingCamera(GameObject *gameObject)
        : Component(gameObject)
{
    setZoomMode(false);
}

Camera &SideScrollingCamera::getCamera() {
    return camera;
}

void SideScrollingCamera::update(float deltaTime) {
    if (followObject != nullptr){
		
        auto position = followObject->getPosition();
        position.x += offset.x;
        position.y += offset.y;
        if (zoom) {
            position.x -= offset.x / 2;
            position.y -= offset.y / 2;
        }
        
        //If the camera has not reached the player, add lerp, otherwise reset
        if (length(lastPosition - position) > 0.1f) {
            lerpTime += deltaTime;
		}
        else
        {
            lerpTime = 0;
        }

        vec2 targetPos = vec2lerp(position, lastPosition, lerpTime);
		
        gameObject->setPosition(targetPos);
        vec3 eye (targetPos, 0);
        vec3 at (targetPos, -1);
        vec3 up (0, 1, 0);
        camera.lookAt(eye, at, up);

        lastPosition = targetPos;
    }
}

vec2 SideScrollingCamera::vec2lerp(vec2& a, vec2& b, float t)
{
    if (t > 1.0f)
    {
		t = 1.0f;
    }
	
    //Lerp between two vectors a and b by t
    return a * t + b * (1.f - t);
}

void SideScrollingCamera::setFollowObject(shared_ptr<GameObject> followObject, vec2 offset) {
    this->followObject = followObject;
    this->offset = offset;
}

void SideScrollingCamera::setZoomMode(bool zoomEnabled) {
    this->zoom = zoomEnabled;
    if (zoomEnabled){
        camera.setOrthographicProjection(PlatformerGame::windowSize.y/4,-1,1);
    } else {
        camera.setOrthographicProjection(PlatformerGame::windowSize.y/2,-1,1);
    }
}

bool SideScrollingCamera::isZoomMode() {
    return zoom;
}
