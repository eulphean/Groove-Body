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

    // Setup camera.
    cam.disableMouseInput();
    cam.setFarClip(15000);
    destPosIdx = 0;
    camState = CamState::Far;
    cam.setPosition(camPosition[destPosIdx]); // Set initial camera position for the camera.
    camTime = ofGetElapsedTimeMillis();
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
    gui.setPosition(glm::vec2(50, 65));
  
    // Set tilt angle.
    cam.tiltDeg(tiltCamera);
  
    // Setup light.
    light.setPosition(xLight, yLight, zLight);
    light.setDiffuseColor(ofFloatColor::gold);
    light.setSpecularColor(ofFloatColor::greenYellow);
  
    // Initialize the current form
    currentForm.setup(formPaths[formPathIdx]);
}

//--------------------------------------------------------------
void ofApp::update(){
  // Update light position
  light.setPosition(xLight, yLight, zLight);

  // Update the form for animation.
  currentForm.update();
  
  // Update camera
  updateCamera();
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
  cam.begin();
    ofEnableLighting();
    light.enable();
  
    // Draw the model.
    currentForm.draw();
  
    light.disable();
    ofDisableLighting();
  cam.end();
  ofDisableDepthTest();
}

void ofApp::updateCamera() {
  auto elapsedTime = ofGetElapsedTimeMillis() - camTime;
  
  if (elapsedTime > 2000 && isCamMoving == false) {
    // Set the next state.
    if (camState == CamState::Far) {
      camState = CamState::Close;
      destPosIdx = 1;
    } else if (camState == CamState::Close) {
      camState = CamState::SuperClose;
      destPosIdx = 2;
    } else if (camState == CamState::SuperClose) {
      camState = CamState::Orbit;
    }
  }
  
  auto destPos = camPosition[destPosIdx];
  auto curCamPos = cam.getPosition();
  if (curCamPos != destPos) {
    moveCamera(destPosIdx);
  }
}

void ofApp::moveCamera(int destPosIdx) {
  auto destPos = camPosition[destPosIdx];
  auto curCamPos = cam.getPosition();
  
  // Update x
  if (curCamPos.x != destPos.x) {
    curCamPos.x = curCamPos.x - 1;
  }
  // Update y
  if (curCamPos.y != destPos.y) {
    curCamPos.y = curCamPos.y + 1 ;
  }
  // Update z
  if (curCamPos.z != destPos.z) {
    curCamPos.z = curCamPos.z - 5;
  }
  
  cam.setPosition(curCamPos);
  
  // Are we there yet?
  if (curCamPos == destPos) {
    camTime = ofGetElapsedTimeMillis(); // Save the last time.
    isCamMoving = false;
  } else {
    isCamMoving = true;
  }
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
    
    destPosIdx = 0;
    camState = CamState::Far;
    cam.setPosition(camPosition[destPosIdx]); // Set initial camera position for the camera.
    camTime = ofGetElapsedTimeMillis();
    
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
  cam.tiltDeg(angle);
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
