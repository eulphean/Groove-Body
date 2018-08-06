#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxAssimpUtils.h"
#include "Particle.h"

enum Mesh {
  wireframe,
  vertices,
  faces
};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
  
  private:
    void createParticles();
    void drawParticles();
  
    vector<Particle> particles;
    
    ofxAssimpModelLoader model;
  
    ofEasyCam cam;
    float cameraOrbit;
    ofLight light;
    ofMesh mesh; 
  
    Mesh meshState = Mesh::wireframe;
};
