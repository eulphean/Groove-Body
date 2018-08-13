#include "Coin.h"

void Coin::setup(glm::vec3 initVelocity, glm::vec3 initPosition, float radius) {
  currentPosition = initPosition;
  velocity = initVelocity;
  coinRadius = radius;
}

void Coin::update(glm::vec3 newPosition, float step)
{
    currentPosition = newPosition;
    update(step);
}

void Coin::update(float step) {
    currentPosition = currentPosition + velocity;
    life = life - step;
    angleX += 2;
    angleY += 2;
    angleZ += 2;
}

float Coin::getLife() {
  return life;
}

void Coin::draw()
{
    ofFill();
    ofPushStyle();
      //Change the brightness of the particles with time.
      auto opacity = ofMap(life, 1, 0, 255, 0, true);
      ofSetColor(ofColor::gold, opacity);
      ofPushMatrix();
        ofTranslate(currentPosition);
        ofRotateXDeg(angleX);
        ofRotateYDeg(angleY);
        ofRotateZDeg(angleZ);
        ofDrawCylinder(0, 0, coinRadius, 0.0002);
      ofPopMatrix();
    ofPopStyle();
}
