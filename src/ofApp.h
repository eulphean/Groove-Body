#ifndef _OF_APP
#define _OF_APP

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Form.h"
#include "ofxGui.h"

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
  
    ofFbo fbo;
  
    // Plane
    ofPlanePrimitive plane; 
  
    // Hide/Show things
    bool hideControls = false;
    bool fboDraw = false;
    int screenCaptureIdx = 0;
  
    // GUI
    ofxPanel gui;
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
};

#endif
