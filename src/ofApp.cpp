#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Verbose, Notice, Warning, Error, FatalError, Silent
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
  
    ofSetCylinderResolution(10, 10);
  
    ofBackground(ofColor::black);
  
    // Load all the forms. 
    loadForms();

    // Setup camera.
    cam.disableMouseInput();
    cameraOrbit = 0;
  
    // Setup plane.
    plane.set(1000, 1000);
    plane.setPosition(0, 0, 0);
    plane.rotateDeg(90, 1, 0, 0);
    plane.setResolution(10, 10);
  
    // Setup GUI.
    gui.setup();
    gui.add(xCamera);
    gui.add(yCamera);
    gui.add(zCamera);
    gui.add(tiltCamera);
    gui.add(xLight);
    gui.add(yLight);
    gui.add(zLight);
    gui.add(wireframe.setup("Wireframe", false));
    gui.add(faces.setup("Faces", false));
    gui.add(vertices.setup("Vertices", false));
    gui.add(particles.setup("Particles", false));
  
    // Listeners
    tiltCamera.addListener(this, &ofApp::cameraTiltCallback);
    wireframe.addListener(this, &ofApp::wireframeCallback);
    faces.addListener(this, &ofApp::facesCallback);
    vertices.addListener(this, &ofApp::verticesCallback);
    particles.addListener(this, &ofApp::particlesCallback);
  
    gui.loadFromFile(guiXml);
    gui.setPosition(glm::vec2(50, 55));
  
    // Set tilt angle.
    cam.tiltDeg(tiltCamera);
  
    // Setup light.
    light.setPosition(xLight, yLight, zLight);
    light.setDiffuseColor(ofFloatColor::gold);
    light.setSpecularColor(ofFloatColor::greenYellow);
  
    // Initialize the current form
    forms[curFormIdx].initialize();
}

//--------------------------------------------------------------
void ofApp::update(){
  // Update camera position.
  cam.setPosition(xCamera, yCamera, zCamera);
  
  // Update light position
  light.setPosition(xLight, yLight, zLight);

//  cameraOrbit += ofGetLastFrameTime() * 10.; // 20 degrees per second;
//  cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), {0., 0., 0.});
//  glm::vec3 formCenter = forms[curFormIdx].model.getSceneCenter();
//  cam.orbitDeg(cameraOrbit, 0., glm::distance(cam.getPosition(), formCenter), formCenter);

  // Update the form for animation.
  forms[curFormIdx].update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  // Controls and debug.
  if (!hideControls) {
    gui.draw();
    // Debug text
    
    ofDrawBitmapStringHighlight("Vertices: " +  ofToString(forms[curFormIdx].getMeshVertexCount()),50, 10);
    ofDrawBitmapStringHighlight("Frame Rate: " + ofToString(ofGetFrameRate()), 50, 40);
    ofDrawBitmapStringHighlight("Dynamic Particle Count: " + ofToString(forms[curFormIdx].getDynamicParticleCount()), 50, 25);
  }
  
  
  ofEnableDepthTest();
  cam.begin();
    ofEnableLighting();
    light.enable();
  
    // Draw the model.
    forms[curFormIdx].draw();
  
    // Draw the light
    if (!hideControls) {
      ofPushStyle();
        ofPushMatrix();
          ofTranslate(light.getPosition());
          ofSetColor(ofColor::red);
          ofDrawSphere(0, 0, 0, 1);
        ofPopMatrix();
      ofPopStyle();
    }
  
    light.disable();
    ofDisableLighting();
  cam.end();
  ofDisableDepthTest();
}

void ofApp::exit() {
  gui.saveToFile(guiXml);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == ' ') {
    // Clean the coin memory from the previous form.
    forms[curFormIdx].cleanMemory();
    
    // Next form.
    curFormIdx = (curFormIdx + 1) % forms.size();
    
    // Initialize the form.
    forms[curFormIdx].initialize();
    
    // Push current draw mode state.
    if (wireframe == true) {
      forms[curFormIdx].pushDrawMode(DrawMode::Wireframe);
    }
    
    if (particles == true) {
      forms[curFormIdx].pushDrawMode(DrawMode::Particles);
    }
    
    if (faces == true) {
      forms[curFormIdx].pushDrawMode(DrawMode::Faces);
    }
    
    if (vertices == true) {
      forms[curFormIdx].pushDrawMode(DrawMode::Vertices);
    }
    
    // Reset animation to start from the beginning. 
    forms[curFormIdx].model.resetAllAnimations();
  }
  
  if (key == 'h') {
    hideControls = !hideControls; 
  }
}

void ofApp::loadForms() {
  // Load all the models.
  ofDirectory dir("Forms/");
  dir.allowExt("dae");
  dir.listDir();
  for (int i = 0; i < dir.size(); i++) {
    string filePath = dir.getPath(i);
    Form form(filePath); // Create a form.
    forms.push_back(form);
  }
  
  ofLogNotice() << "Success: All the dae animations loaded." << "\n";
}

// GUI callbacks.
void ofApp::cameraTiltCallback(float &angle) {
  cam.tiltDeg(angle);
}

void ofApp::wireframeCallback(bool &value) {
  if (value) {
    forms[curFormIdx].pushDrawMode(DrawMode::Wireframe);
  } else {
    forms[curFormIdx].popDrawMode(DrawMode::Wireframe);
  }
}

void ofApp::facesCallback(bool &value) {
  if (value) {
    forms[curFormIdx].pushDrawMode(DrawMode::Faces);
  } else {
    forms[curFormIdx].popDrawMode(DrawMode::Faces);
  }
}

void ofApp::verticesCallback(bool &value) {
  if (value) {
    forms[curFormIdx].pushDrawMode(DrawMode::Vertices);
  } else {
    forms[curFormIdx].popDrawMode(DrawMode::Vertices);
  }
}

void ofApp::particlesCallback(bool &value) {
  if (value) {
    forms[curFormIdx].pushDrawMode(DrawMode::Particles);
  } else {
    forms[curFormIdx].popDrawMode(DrawMode::Particles);
  }
}
