#include "Particle.h"

void Particle::update(float step)
{
    //calculateNewVelocity();
    currentPosition = currentPosition + velocity;
    life = life - step;
    angleX += 2;
    angleY += 2;
    angleZ += 2;
}

void Particle::draw()
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
        ofDrawCylinder(0, 0, 0.0009, 0.0002);
      ofPopMatrix();
    ofPopStyle();
}
