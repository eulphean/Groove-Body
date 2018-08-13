#pragma once

#include "ofMain.h"

class Coin
{
  public:
    void setup(glm::vec3 initVelocity, glm::vec3 currentPos, float coinRadius, float coinHeight);
    void update(float step);
    void update(glm::vec3 newPosition, float step);
    void draw();
    float getLife();
  
  private:
    glm::vec3 velocity;
    glm::vec3 currentPosition;
    float coinRadius = 0.0;
    float coinHeight = 0.0;
  
    float angleX = ofRandom(0, 360);
    float angleY = ofRandom(0, 360);
    float angleZ = ofRandom(0, 360);
  
    float life = 1.0f;
};
