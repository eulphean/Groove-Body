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
  
    void reset() {
      this->setup();
    }
  
    virtual void setup() { }
    virtual void update() { }
    glm::vec3 sceneCenter;
    glm::vec3 sceneMin;
    glm::vec3 sceneMax;
    glm::vec3 head;
    CamState camState;
};

class A1Cam : public CameraProfile {
  public:
    void setup() {
      startPos = glm::vec3(-200, -500 , 0);
      targetPos = glm::vec3(0, sceneCenter.y, 2000);
      camState = CamState::Tween; // Starting state.
      orbit = 0; 
      // Order of the states is Tween -> Hold -> Orbit
    }
  
    void update() {
      switch (camState) {
        case CamState::Tween: {
          glm::vec3 lerpPos;
          startPos.x = ofLerp(startPos.x, targetPos.x, 0.002);
          startPos.y = ofLerp(startPos.y, targetPos.y, 0.002);
          startPos.z = ofLerp(startPos.z, targetPos.z, 0.002);
         
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
            orbit = this->getHeadingDeg();
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
    const long int maxHoldTime = 2000;
    float orbit;
};

class A2Cam : public CameraProfile {
  public:
    void setup() {
      startPos = glm::vec3(800, head.y, 5000);
      targetPos = glm::vec3(800, sceneCenter.y, 1600);
      camState = CamState::Tween; // Starting state.
      orbit = 0;
      // Order of the states is Tween -> Hold -> Orbit
    }
  
    void update() {
      switch (camState) {
        case CamState::Tween: {
          glm::vec3 lerpPos;
          startPos.x = ofLerp(startPos.x, targetPos.x, 0.005);
          startPos.y = ofLerp(startPos.y, targetPos.y, 0.005);
          startPos.z = ofLerp(startPos.z, targetPos.z, 0.005);
         
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
            orbit = this->getHeadingDeg();
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
    const long int maxHoldTime = 2000;
    float orbit;
};

class A3Cam : public CameraProfile {
  public:
    void setup() {
      startPos = glm::vec3(500, sceneCenter.y, -5000);
      targetPos = glm::vec3(1000, sceneCenter.y, 1200);
      camState = CamState::Tween; // Starting state.
      orbit = 0;
      // Order of the states is Tween -> Hold -> Orbit
    }
  
    void update() {
      switch (camState) {
        case CamState::Tween: {
          glm::vec3 lerpPos;
          startPos.x = ofLerp(startPos.x, targetPos.x, 0.002);
          startPos.y = ofLerp(startPos.y, targetPos.y, 0.002);
          startPos.z = ofLerp(startPos.z, targetPos.z, 0.002);
         
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
            orbit = this->getHeadingDeg();
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

class A4Cam : public CameraProfile {
  public:
    void setup() {
      startPos = glm::vec3(0, head.y + 5000, -5000);
      targetPos = glm::vec3(1000, sceneCenter.y, 1500);
      camState = CamState::Tween; // Starting state.
      orbit = 0;
      // Order of the states is Tween -> Hold -> Orbit
    }
  
    void update() {
      switch (camState) {
        case CamState::Tween: {
          glm::vec3 lerpPos;
          startPos.x = ofLerp(startPos.x, targetPos.x, 0.002);
          startPos.y = ofLerp(startPos.y, targetPos.y, 0.002);
          startPos.z = ofLerp(startPos.z, targetPos.z, 0.002);
         
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
            this->setup();
          }
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
