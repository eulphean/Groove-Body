#include "Particle.h"

void Particle::update()
{
    angularVelocity = angularVelocity + angularAcceleration;
    orientation = orientation + angularVelocity;

    velocity = velocity + acceleration;
    position = position + velocity;
}

void Particle::draw()
{
    ofFill();
    ofSetColor(color);

    ofPushMatrix();
      ofTranslate(position);
      ofRotateXDeg(orientation.x);
      ofRotateYDeg(orientation.y);
      ofRotateZDeg(orientation.z);

      ofDrawCircle(0, 0, radius);
    ofPopMatrix();
}
