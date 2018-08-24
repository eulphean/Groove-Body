#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Verbose, Notice, Warning, Error, FatalError, Silent
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    
    ofDisableArbTex();
  
    ofEnableAntiAliasing();
    ofEnableSmoothing();
  
    ofSetCylinderResolution(10, 10);
  
    ofColor backgroundColor = ofColor(9, 10, 11);
    ofBackground(backgroundColor);
  
    // Load all the forms. 
    loadForms();
  
    // Setup GUI.
    gui.setup();
    gui.add(currentForm.xCoinLight);
    gui.add(currentForm.yCoinLight);
    gui.add(currentForm.zCoinLight);
    gui.add(currentForm.xMeshLight);
    gui.add(currentForm.yMeshLight);
    gui.add(currentForm.zMeshLight);
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
  
    // Setup fbo.
    fbo.allocate(5782, 3946, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::update(){
  // Update the form for animation.
  currentForm.update();
  
  // Only for screen capturing.
  if (fboDraw) {
    fbo.begin();
      ofClear(ofColor::black);
      ofEnableDepthTest();
      // Draw the model.
      currentForm.draw();
    
      ofDisableDepthTest();
    fbo.end();
  }
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
  
  if (fboDraw) {
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
  } else {
    ofEnableDepthTest();
      currentForm.draw();
    ofDisableDepthTest();
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
  
  if (key == 'e') {
    currentForm.emitCoins();
  }
  
  //////// --------- High-Res capture. First enable fbo draw. Then 's' to to save the image. //////
  if (key == 'f') {
    fboDraw = !fboDraw;
  }
  
  if (key == 's') {
    ofPixels pix;
    fbo.readToPixels(pix);
    auto fileName = "High_Res" + ofToString(screenCaptureIdx) + ".png";
    screenCaptureIdx++;
    ofSaveImage(pix, fileName, OF_IMAGE_QUALITY_BEST);
  }
  
  // Reset the entire model. 
  if (key == 'r') {
    currentForm.resetCam();
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
