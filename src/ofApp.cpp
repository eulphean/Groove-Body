#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(ofColor::black);
  ofSetVerticalSync(true);
  
  // Load human model.
  model.loadModel("basicman.obj", false);
  
  // Center the model.
  model.setPosition(0, 0, 0);
  model.setRotation(0, 180, 0, 0, 1);
  model.setScale(1.5, 1.5, 1.5);
  
  // Setup light.
  light.setPosition(0, 0, 500);
  
  // Setup camera.
  cameraOrbit = 0;
  cam.setDistance(1000);
}

//--------------------------------------------------------------
void ofApp::update(){
  cameraOrbit += ofGetLastFrameTime() * 20.; // 20 degrees per second;
  cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), {0., 0., 0.});
}

//--------------------------------------------------------------
void ofApp::draw(){
  // Draw the frame rate.
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), 50, 50);
  
  ofEnableDepthTest();
  cam.begin();
  ofDrawAxis(10);
  ofSetColor(ofColor::grey);
  
  // Mesh draw.
  switch (meshState) {
    case Mesh::vertices: {
      model.drawVertices();
      break;
    }
    
    case Mesh::wireframe: {
      model.drawWireframe();
      break;
    }
    
    case Mesh::faces: {
      model.drawFaces();
      break;
    }
    
    default: {
      break;
    }
  }
  
  cam.end();
  ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (meshState == Mesh::wireframe) {
    meshState = Mesh::vertices;
  } else if (meshState == Mesh::vertices) {
    meshState = Mesh::faces;
  } else if (meshState == Mesh::faces) {
    meshState = Mesh::wireframe;
  }
}
