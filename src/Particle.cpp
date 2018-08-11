#include "Particle.h"

void Particle::update(float step)
{
    //calculateNewVelocity();
    currentPosition = currentPosition + velocity;
    life = life - step;
}

void Particle::draw()
{
    ofFill();
    ofPushStyle();
      //Change the brightness of the particles with time.
      auto opacity = ofMap(life, 1, 0, 255, 0, true);
      ofSetColor(color, opacity);
      ofPushMatrix();
        ofTranslate(currentPosition);
        ofDrawSphere(0, 0, radius);
      ofPopMatrix();
    ofPopStyle();
}
