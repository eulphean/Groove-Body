#pragma once

#include "ofMain.h"

class Coin : public ofNode {
  public:
    void setup(bool isThisStatic, glm::vec3 position) {
      this->isStatic = isThisStatic;
      this->life = 1.0;
      this->curRotation = 0;
      this->setPosition(position);
      rotAxis = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)); // Pick a standard rotation axis.
      
      // Static vs. Flying coin settings.
      if (isStatic) {
        rotation = 0.1; // Pick a slow random rotation value. Don't want static coins to be rotating too quickly.
        this->velocity = glm::vec3(0, 0, 0);
        this->setScale(10.0); // size.
      } else {
        rotation = 2.0; // Rotation can be bigger for this.
        this->setScale(10.0);
        this->acceleration = 0.003;
      }
    }
  
    void update(float step) {
      this->setPosition(this->getPosition() + velocity);
      this->rotateDeg(rotation, rotAxis);
      
      if (!isStatic) {
        auto f = this->velocity * this->acceleration;
        this->velocity += f;
      } else {
        this->curRotation+= this->rotation;
        // If it's a static coin, we must reset its transform else it scales and becomes really weird. 
        if (this->curRotation == 360) {
          auto curPos = this->getPosition();
          this->resetTransform();
          this->setup(false, curPos);
        }
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
    float curRotation;
  
    float life = 1.0;
  
};
