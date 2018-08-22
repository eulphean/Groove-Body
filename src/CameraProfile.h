#pragma once

#include "ofMain.h"

enum CamState {
  Tween,
  Orbit,
  Hold
};

class CameraProfile : public ofCamera {
  public:
    void init(glm::vec3 center, glm::vec3 min, glm::vec3 max, glm::vec3 h) {
      sceneCenter = center;
      sceneMin = min;
      sceneMax = max;
      head = h;
      setup();
    }
  
    virtual void setup() { }
    virtual void update() { }
    glm::vec3 sceneCenter;
    glm::vec3 sceneMin;
    glm::vec3 sceneMax;
    glm::vec3 head;
    CamState camState;
};

class BreathCam : public CameraProfile {
  public:
    void setup() {
      startPos = glm::vec3(0, -100, 0);
      targetPos = glm::vec3(0, sceneCenter.y, 1650);
      camState = CamState::Tween; // Starting state.
      orbit = 0; 
      // Order of the states is Tween -> Hold -> Orbit
    }
  
    void update() {
      switch (camState) {
        case CamState::Tween: {
          glm::vec3 lerpPos;
          startPos.x = ofLerp(startPos.x, targetPos.x, 0.001);
          startPos.y = ofLerp(startPos.y, targetPos.y, 0.001);
          startPos.z = ofLerp(startPos.z, targetPos.z, 0.001);
         
          this->setPosition(startPos);
          this->lookAt(sceneCenter, glm::vec3(0, 1, 0));
          
          if (glm::distance(startPos, targetPos) < 150) {
            camState = CamState::Hold;
            initHoldTime = ofGetElapsedTimeMillis();
          }
          break;
        }
        
        case CamState::Hold: {
          auto hasElapsed = (ofGetElapsedTimeMillis() - initHoldTime) > maxHoldTime;
          if (hasElapsed) {
            camState = CamState::Orbit;
          }
          break;
        }
        
        case CamState::Orbit: {
          orbit += ofGetLastFrameTime() * 2;
          this->orbitDeg(orbit, 0, glm::distance(this->getPosition(), sceneCenter), sceneCenter);
          break;
        }
      }
    }
  
    glm::vec3 startPos;
    glm::vec3 targetPos;
    unsigned long int initHoldTime;
    const long int maxHoldTime = 5000;
    float orbit;
};
