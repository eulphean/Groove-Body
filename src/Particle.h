#pragma once

#include "ofMain.h"

class Particle
{
  public:
    void update(float step);
    void draw();

    glm::vec3 velocity;
    glm::vec3 currentPosition;
    float life = 1.0f;
    float radius = 0.0;
    float velocityConstant = 0.001;
    ofColor color;
};
