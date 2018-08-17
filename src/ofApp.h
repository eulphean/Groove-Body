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
    unsigned long int camTime;
    bool isCamMoving = false;
    int destPosIdx = 0; 
  
    // Hide/Show things
    bool hideControls = false;
  
    // Camera state
    CamState camState = CamState::Far;
    // For first three states. 
    vector<glm::vec3> camPosition = { glm::vec3(0, 0, 5000), glm::vec3(0, 300, 2000), glm::vec3(-150, 600, 900) };
  
    // GUI
    ofxPanel gui;
    ofParameter<float> xCamera { "x: camera", 0, -5000, 5000 }; 
    ofParameter<float> yCamera { "y: camera", 0, -5000, 5000 };
    ofParameter<float> zCamera { "z: camera", 20, -5000, 10000 };
    ofParameter<float> tiltCamera { "tilt: camera", 0, -180, 180 };
    ofParameter<float> xLight { "x: light", 0, -10000, 10000 };
    ofParameter<float> yLight { "y: light", 0, -10000, 10000 };
    ofParameter<float> zLight { "z: light", 8000, -10000, 10000 };
    // Model parameters
    ofxToggle wireframe; ofxToggle faces; ofxToggle vertices; ofxToggle particles;
    const string guiXml = "humanform.xml";
  
    // GUI callbacks
    void cameraTiltCallback(float &angle);
    void wireframeCallback(bool &value);
    void facesCallback(bool &value);
    void verticesCallback(bool &value);
    void particlesCallback(bool &value);
  
    // Helpers
    void loadForms();
    void updateCamera();
    void moveCamera(int destPosIdx);
};

#endif
