#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxAssimpUtils.h"
#include "Particle.h"

enum Mesh {
  wireframe,
  vertices,
  faces,
  noFaces
};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
  
  private:
    void createFixedParticles();
    void createDynamicParticles();
    void drawParticles();
  
    vector<Particle> fixedParticles;
    vector<Particle> dynamicParticles;
    
    ofxAssimpModelLoader model;
    ofMesh mesh;
    Mesh meshState = Mesh::noFaces;
  
    ofEasyCam cam;
    float cameraOrbit;
};
