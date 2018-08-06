#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

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
};
