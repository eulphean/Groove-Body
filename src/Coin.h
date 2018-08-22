#pragma once

#include "ofMain.h"

class Coin : public ofNode {
  public:
    void update(float step) {
      this->setPosition(this->getPosition() + velocity);
      this->rotate(rotation, 1.0, 0, 0.0); // Constant rotation.
      this->rotate(rotation, 0.0, 1.0, 0.0); // Constant rotation.
      this->rotate(rotation, 0.0, 0, 1.0); // Constant rotation.
      life = life - step;
    }
  
    bool isAlive() {
      return life >= 0.0;
    }
  
    glm::vec3 velocity;
    float life = 1.0;
    float rotation = ofRandom(-0.5, 0.5);
};
