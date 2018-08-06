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
  cam.setDistance(20);
  
  // Retrieve ofMesh
  mesh = model.getMesh(0);
  cout << "Vertices " << mesh.getVertices().size() << "\n";
  
  // Create particles at each mesh vertex
  createParticles();
}

//--------------------------------------------------------------
void ofApp::update(){
  ofRectangle screenRect(0, 0, ofGetWidth(), ofGetHeight());
  cameraOrbit += ofGetLastFrameTime() * 10.; // 20 degrees per second;
  cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), {0., 0., 0.});
  for (auto &p: particles) {
    p.update();
    if (screenRect.inside(p.position.x, p.position.y) == false) {
      cout << "Particle not here" << "\n";
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  // Draw the frame rate.
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), 50, 50);
  
  ofEnableDepthTest();
  cam.begin();
  ofDrawAxis(2);
  ofSetColor(ofColor::grey);
  
  ofPushMatrix();
  
   // Scale the matrix to the size of the model.
  //ofMultMatrix(model.getModelMatrix());
  
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
  
  ofPopMatrix();
  
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
      meshState = Mesh::wireframe;
    }
  }
  
  if (key == ' ')  {
    if (mesh.getMode() == OF_PRIMITIVE_TRIANGLES) {
      mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY);
    } else if (mesh.getMode() == OF_PRIMITIVE_TRIANGLE_STRIP) {
      mesh.setMode(OF_PRIMITIVE_LINE_STRIP_ADJACENCY);
    } else if (mesh.getMode() == OF_PRIMITIVE_LINE_STRIP_ADJACENCY) {
      mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    }
  }
}

void ofApp::createParticles() {
  particles.clear();
  for (int i = 0; i < mesh.getVertices().size(); i+=3) {
    Particle p;
    // Initial position
    p.position = mesh.getVertices()[i];

    // Set initial velocities.
    //p.velocity = glm::vec3(ofRandom(-0.1, 0.1), ofRandom(-0.1, 0.1) , ofRandom(-0.1, 0.1));
    p.velocity = glm::vec3(0, 0, 0);
    
    // Set inital accelerations.
    p.acceleration = glm::vec3(0, 0, 0);

    // Set angular velocity.
    p.angularVelocity.x = ofRandom(0, 0);
    p.angularVelocity.y = ofRandom(0, 0);
    p.angularVelocity.z = ofRandom(0, 0);

    // Set radius.
    p.radius = 0.05;

    // Set color.
    p.color = ofColor::fromHsb(ofRandom(255), 255, 255);

    // Add a copy to our vector.
    particles.push_back(p);
  }
}

void ofApp::drawParticles() {
 for (Particle& p: particles)
  {
      p.draw();
  }
}
