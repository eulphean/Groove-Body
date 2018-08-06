#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(ofColor::black);
  ofSetVerticalSync(true);
  
  // Load human model.
  model.loadModel("basicman.obj", false);
  
  // Center the model.
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
  model.setPosition(0, 0, 0);
  
  cameraOrbit += ofGetLastFrameTime() * 20.; // 20 degrees per second;
  cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), {0., 0., 0.});
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofEnableDepthTest();
  cam.begin();
  ofDrawAxis(10);
  ofSetColor(ofColor::grey);
  model.drawVertices();
  cam.end();
  ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
