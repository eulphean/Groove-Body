#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

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
    ofxAssimpModelLoader model;
    ofEasyCam cam;
    float cameraOrbit;
    ofLight light;
  
    Mesh meshState = Mesh::wireframe; 
};
