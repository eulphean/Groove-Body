#include "Particle.h"

void Particle::update(float step)
{
    position = position + velocity;
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
        ofTranslate(position);
        ofDrawSphere(0, 0, radius);
      ofPopMatrix();
    ofPopStyle();
}

void Particle::attractTo(glm::vec3 attractionPoint) {
  glm::vec3 dir = attractionPoint - position;
  float distance = glm::distance(attractionPoint, position);
  // 2 units is just a minimum distance to keep in mind.
  if (distance > 5.0) {
    velocity += glm::normalize(dir);
  } else {
    cout << "Particle is here" << "\n";
    velocity = glm::vec3(0, 0, 0);
  }
}
