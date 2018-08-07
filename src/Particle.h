#pragma once

#include "ofMain.h"

class Particle
{
public:
    void update(float step);
    void draw();
    void attractTo(glm::vec3 attractionPoint);

    glm::vec3 position;
    glm::vec3 velocity;
  
    float life = 1.0f;
    float radius = 0.0;
    ofColor color;
};
