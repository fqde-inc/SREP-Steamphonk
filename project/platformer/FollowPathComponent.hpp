#pragma once

#include "Component.hpp"
#include <vector>
#include "glm/glm.hpp"

using namespace glm;
using namespace std;

enum PathType { BEZIER, CATMULL_ROW, LINEAR };

class FollowPathComponent : public Component{
public:
    explicit FollowPathComponent(GameObject *gameObject);

    void update(float deltaTime) override;

    void setPositions(vector<vec2> positions);;
    void setType(PathType _type){type = _type;};

    // Public for debugging purposes
    vec2 computePositionAtTime(float time);
    int getNumberOfSegments();
    
private:

    vec2 getBezierPosition(vec2 p0, vec2 p1, vec2 p2, float t);
    vec2 getCatmullPosition(vec2 p0, vec2 p1, vec2 p2, vec2 p3, float t, float tension);

    vector<vec2> positions;

    shared_ptr<Component> spriteRef; 

    PathType type;

    float time = 0;
    int lastSegment = 0;
    bool looping = false;

    float flapTime = 1.0f;

    float radius;
};
