#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetVerticalSync(false);
  ofBackground(ofColor::black);

  
  // Load human model.
  model.loadModel("basicman2.obj", false);
  
  // Center the model.
  model.setPosition(0, 0, 0);
  model.setRotation(0, 180, 0, 0, 1);
  model.setScale(1.5, 1.5, 1.5);
  
  // Setup camera.
  cameraOrbit = 0;
  cam.setDistance(20);
  
  // Retrieve ofMesh
  mesh = model.getMesh(0);
  cout << "Vertices " << mesh.getVertices().size() << "\n";
  
  // Create particles at each mesh vertex
  createFixedParticles();
}

//--------------------------------------------------------------
void ofApp::update(){
  cameraOrbit += ofGetLastFrameTime() * 10.; // 20 degrees per second;
  cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), {0., 0., 0.});
  
  if (dynamicParticles.size() > 0) {
    auto iter = dynamicParticles.begin();
    
    while (iter != dynamicParticles.end()) {
      if (iter -> life > 0) {
        float step = ofGetLastFrameTime()/ofRandom(20,30);
        iter -> update(step);
        iter++;
      } else {
        iter = dynamicParticles.erase(iter);
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofEnableDepthTest();
  // Draw the frame rate.
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), 50, 50);
  ofDrawBitmapStringHighlight("Total Particles: " + ofToString(fixedParticles.size() + dynamicParticles.size()), 50, 65);
  
  cam.begin();
  
  // Mesh draw.
  switch (meshState) {
    case Mesh::vertices: {
      mesh.drawVertices();
      break;
    }
    
    case Mesh::wireframe: {
      mesh.drawWireframe();
      break;
    }
    
    case Mesh::faces: {
      mesh.drawFaces();
      break;
    }
    
    default: {
      break;
    }
  }
  
  drawParticles();
  
  cam.end();
  ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  // Change the state to draw.
  if (key == OF_KEY_RETURN) {
    if (meshState == Mesh::wireframe) {
      meshState = Mesh::vertices;
    } else if (meshState == Mesh::vertices) {
      meshState = Mesh::faces;
    } else if (meshState == Mesh::faces) {
      meshState = Mesh::noFaces;
    } else if (meshState == Mesh::noFaces) {
      meshState = Mesh::wireframe;
    }
  }
  
  // Create new particles
  if (key == 'c') {
    createDynamicParticles();
  }
}

void ofApp::createFixedParticles() {
  fixedParticles.clear();
  for (int i = 0; i < mesh.getVertices().size(); i++) {
    Particle p;
    p.position = mesh.getVertices()[i];
    p.velocity = glm::vec3(0, 0, 0);
    p.radius = 0.05;
    p.color = ofColor::fromHsb(ofRandom(255), 255, 255);

    // Add a copy to our vector.
    fixedParticles.push_back(p);
  }
}

void ofApp::createDynamicParticles() {
  for (int i = 0; i < mesh.getVertices().size(); i++) {
    Particle p;
    p.position = mesh.getVertices()[i];
    p.velocity = glm::vec3(ofRandom(-0.005, 0.005), ofRandom(-0.005, 0.005), ofRandom(-0.01, 0.01));
    p.radius = 0.1;
    p.color = ofColor::fromHsb(ofRandom(255), 255, 255);
    dynamicParticles.push_back(p);
  }
}

void ofApp::drawParticles() {
 for (Particle &p: fixedParticles) { p.draw(); }
 for (Particle &p: dynamicParticles) { p.draw(); }
}
