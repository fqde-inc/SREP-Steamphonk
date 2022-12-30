#pragma once

#include "sre/Camera.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"

using namespace std;
using namespace sre;
using namespace glm;

class SideScrollingCamera : public Component {
public:
    explicit SideScrollingCamera(GameObject *gameObject);

    void update(float deltaTime) override;

    void setFollowObject(shared_ptr<GameObject> followObject, vec2 offset);

    void setZoomMode(bool zoomEnabled);
    bool isZoomMode();

    vec2 offset;
    	
    //vec2 pixelToCameraSpace(vec2& pixelCoordinates);
    Camera& getCamera();
private:
    Camera camera;
    shared_ptr<GameObject> followObject;
    bool zoom = false;
    float lerpTime = 0.f;
    vec2 lastPosition;
    vec2 vec2lerp(vec2& a, vec2& b, float t);
};

