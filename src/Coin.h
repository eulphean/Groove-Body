#pragma once

#include "ofMain.h"

class Coin : public ofNode {
  public:
    void update(float step) {
      this->setPosition(this->getPosition() + velocity);
      this->rotate(rotation, rotAxis); // Constant rotation.
      life = life - step;
    }
  
    bool isAlive() {
      return life >= 0.0;
    }
  
    glm::vec3 velocity;
    float life = 1.0;
    float rotation = ofRandom(-0.2, 0.2); // Pick a random rotation value. 
    glm::vec3 rotAxis = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)); // Pick a random rotation axis
};
