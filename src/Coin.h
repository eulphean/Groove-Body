#pragma once

#include "ofMain.h"

class Coin : public ofNode {
  public:
    void setup(bool isStatic, glm::vec3 position) {
      this->setPosition(position);
      rotAxis = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)); // Pick a standard rotation axis.
      
      // Static vs. Flying coin settings.
      if (isStatic) {
        rotation = ofRandom(-0.1, 0.1); // Pick a slow random rotation value. Don't want static coins to be rotating too quickly.
        this->velocity = glm::vec3(0, 0, 0);
        this->setScale(10.0); // size.
      } else {
        rotation = ofRandom(-1.0, 1.0); // Rotation can be bigger for this. 
        this->setScale(8.0);
        this->acceleration = 0.001;
      }
    }
  
    void update(float step) {
      this->setPosition(this->getPosition() + velocity);
      this->rotate(rotation, rotAxis);
      
      if (!isStatic) {
        auto f = this->velocity * this->acceleration;
        this->velocity += f;
      }
      
      life = life - step;
    }
  
    bool isAlive() {
      return life >= 0.0;
    }
  
    glm::vec3 velocity;
    bool isStatic;
    float rotation;
    float acceleration;
    glm::vec3 rotAxis; // Pick a random rotation axis
  
    float life = 1.0;
  
};
