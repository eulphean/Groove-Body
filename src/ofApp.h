#ifndef _OF_APP
#define _OF_APP

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Form.h"
#include "ofxGui.h"

enum CamState {
  Far,
  Close,
  SuperClose,
  Orbit
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void exit();
    void keyPressed(int key);
  
    // Helpers.
    void loadForms();
  
  private:
    // All the models.
    Form currentForm;
    int formPathIdx = 0;
    vector<string> formPaths;
  
    // Light
    ofLight	light;
  
    // Plane
    ofPlanePrimitive plane; 
  
    // Camera
    ofEasyCam cam;
    float cameraOrbit;
  
    // Hide/Show things
    bool hideControls = false;
  
    // GUI
    ofxPanel gui;
    ofParameter<float> xCamera { "x: camera", 0, -5000, 5000 }; 
    ofParameter<float> yCamera { "y: camera", 0, -5000, 5000 };
    ofParameter<float> zCamera { "z: camera", 20, -5000, 10000 };
    ofParameter<float> tiltCamera { "tilt: camera", 0, -180, 180 };
    ofParameter<float> xLight { "x: light", 0, -10000, 10000 };
    ofParameter<float> yLight { "y: light", 0, -10000, 10000 };
    ofParameter<float> zLight { "z: light", 200, -10000, 10000 };
    // Model parameters
    ofxToggle wireframe; ofxToggle faces; ofxToggle vertices; ofxToggle particles;
    const string guiXml = "humanform.xml";
  
    // GUI callbacks
    void cameraTiltCallback(float &angle);
    void wireframeCallback(bool &value);
    void facesCallback(bool &value);
    void verticesCallback(bool &value);
    void particlesCallback(bool &value);
};

#endif
