#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Verbose, Notice, Warning, Error, FatalError, Silent
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
  
    ofSetCylinderResolution(10, 10);
  
    ofColor backgroundColor = ofColor(9, 10, 11);
    ofBackground(backgroundColor);
  
    // Load all the forms. 
    loadForms(); 
  
    // Setup plane.
    plane.set(1000, 1000);
    plane.setPosition(0, 0, 0);
    plane.rotateDeg(90, 1, 0, 0);
    plane.setResolution(10, 10);
  
    // Setup GUI.
    gui.setup();
    gui.add(currentForm.xCoinLight);
    gui.add(currentForm.yCoinLight);
    gui.add(currentForm.zCoinLight);
    gui.add(wireframe.setup("Wireframe", false));
    gui.add(faces.setup("Faces", false));
    gui.add(vertices.setup("Vertices", false));
    gui.add(particles.setup("Particles", false));
  
    // Listeners
    wireframe.addListener(this, &ofApp::wireframeCallback);
    faces.addListener(this, &ofApp::facesCallback);
    vertices.addListener(this, &ofApp::verticesCallback);
    particles.addListener(this, &ofApp::particlesCallback);
  
    gui.loadFromFile(guiXml);
    gui.setPosition(glm::vec2(50, 65));
  
    // Initialize the current form
    currentForm.setup(formPaths[formPathIdx]);
}

//--------------------------------------------------------------
void ofApp::update(){
  // Update the form for animation.
  currentForm.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  // Controls and debug.
  if (!hideControls) {
    gui.draw();
    // Debug text
    ofDrawBitmapStringHighlight("Vertices: " +  ofToString(currentForm.getMeshVertexCount()),50, 10);
    ofDrawBitmapStringHighlight("Dynamic Particle Count: " + ofToString(currentForm.getDynamicParticleCount()), 50, 25);
    ofDrawBitmapStringHighlight("Frame Rate: " + ofToString(ofGetFrameRate()), 50, 40);
    ofDrawBitmapStringHighlight("Model name: " + formPaths[formPathIdx], 50, 55);
  }
  
  ofEnableDepthTest();
  // Draw the model.
  currentForm.draw();
  ofDisableDepthTest();
}

void ofApp::exit() {
  gui.saveToFile(guiXml);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == ' ') {
    
    // Deallocate the current form completely.
    currentForm.deallocate();
    
    // Next form.
    formPathIdx = (formPathIdx + 1) % formPaths.size();
  
    
    // Initialize the current form with new model.
    currentForm.setup(formPaths[formPathIdx]);
    
    // Push current draw mode state.
    if (wireframe == true) {
      currentForm.pushDrawMode(DrawMode::Wireframe);
    }
    
    if (particles == true) {
      currentForm.pushDrawMode(DrawMode::Particles);
    }
    
    if (faces == true) {
      currentForm.pushDrawMode(DrawMode::Faces);
    }
    
    if (vertices == true) {
      currentForm.pushDrawMode(DrawMode::Vertices);
    }
    
    // Reset animation to start from the beginning. 
    currentForm.model.resetAllAnimations();
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
    formPaths.push_back(filePath);
  }
  
  ofLogNotice() << "Success: All the dae animations loaded." << "\n";
}

// GUI callbacks.
void ofApp::cameraTiltCallback(float &angle) {
  //cam.tiltDeg(angle);
}

void ofApp::wireframeCallback(bool &value) {
  if (value) {
    currentForm.pushDrawMode(DrawMode::Wireframe);
  } else {
    currentForm.popDrawMode(DrawMode::Wireframe);
  }
}

void ofApp::facesCallback(bool &value) {
  if (value) {
    currentForm.pushDrawMode(DrawMode::Faces);
  } else {
    currentForm.popDrawMode(DrawMode::Faces);
  }
}

void ofApp::verticesCallback(bool &value) {
  if (value) {
    currentForm.pushDrawMode(DrawMode::Vertices);
  } else {
    currentForm.popDrawMode(DrawMode::Vertices);
  }
}

void ofApp::particlesCallback(bool &value) {
  if (value) {
    currentForm.pushDrawMode(DrawMode::Particles);
  } else {
    currentForm.popDrawMode(DrawMode::Particles);
  }
}
