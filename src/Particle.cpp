#include "Particle.h"

void Particle::update(float step)
{
    calculateNewVelocity();
    currentPosition = currentPosition + velocity;
    life = life - step;
}

void Particle::draw()
{
    ofFill();
    ofPushStyle();
      // Change the brightness of the particles with time.
      auto opacity = ofMap(life, 1, 0, 255, 0, true);
      ofSetColor(color, opacity);
      ofPushMatrix();
        ofTranslate(currentPosition);
        ofDrawSphere(0, 0, radius);
      ofPopMatrix();
    ofPopStyle();
}

void Particle::calculateNewVelocity() {
  float distance = glm::distance(finalPosition, currentPosition);
  if (distance > maxDistance) {
    // Pull it in
    auto dir = finalPosition - currentPosition;
    
    // Calculate new direction and set the velocity.
    auto dirVelocity = dir + glm::vec3(ofRandom(-verySmallNumber, verySmallNumber), ofRandom(-verySmallNumber, verySmallNumber), ofRandom(-verySmallNumber, verySmallNumber));
    
    velocity = glm::normalize(dirVelocity) * velocityConstant;
  }
  else if (distance <= minDistance) {
    auto dir = currentPosition - finalPosition;
    
    // Calculate new direction and set the velocity.
    auto dirVelocity = dir + glm::vec3(ofRandom(-verySmallNumber, verySmallNumber), ofRandom(-verySmallNumber, verySmallNumber), ofRandom(-verySmallNumber, verySmallNumber));
    velocity = glm::normalize(dirVelocity) * velocityConstant;
  }
}
