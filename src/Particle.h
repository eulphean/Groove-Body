#pragma once

#include "ofMain.h"

class Particle
{
public:
    void update(float step);
    void draw();
    void calculateNewVelocity();

    // Movement
    glm::vec3 velocity;
  
    // Current position of the particles.
    glm::vec3 currentPosition;
  
    // Real position, where the particle should go towards.
    glm::vec3 finalPosition;
  
    // Min/Max distance from the attraction point.
    float minDistance = 0;
    float maxDistance = 0.3;
    float verySmallNumber = 1;
    float velocityConstant = 0.001; 
  
    float life = 1.0f;
    float radius = 0.0;
    ofColor color;
};
