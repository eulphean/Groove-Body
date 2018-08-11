#include "Form.h"

Form::Form(string modelName) {

  // Load our model and set it up for animations.
  model.loadModel(modelName, false);
  model.setPosition(0, 0, 0);
  model.setRotation(0, 180, 0, 0, 1);
  model.setScale(1.5, 1.5, 1.5);
  model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
  model.setPausedForAllAnimations(false);
  model.playAllAnimations();
  
  // Get the first animated mesh.
  mesh = model.getCurrentAnimatedMesh(0);
  
  // Create inPlaceParticles.
  createInPlaceParticles();
}

void Form::update() {
  // For model animations. 
  model.update();
  mesh = model.getCurrentAnimatedMesh(0);
  
  // Update inPlaceParticle positions.
  auto meshVertices = mesh.getVertices();
  for (int i = 0; i < meshVertices.size(); i++) {
    inPlaceParticles[i].currentPosition = meshVertices[i];
    inPlaceParticles[i].update(0);
  }
  
  // Dynamic/flying particles
  // Set a maximum number of particles probably.
  createDynamicParticles();
  
  // Update dynamic particles and remove them if their life has ended.
  auto iter = dynamicParticles.begin();
  while (iter != dynamicParticles.end()) {
    if (iter -> life > 0) {
      float step = ofGetLastFrameTime()/ofRandom(1,5);
      iter -> update(step);
      iter++;
    } else {
      iter = dynamicParticles.erase(iter);
    }
  }
}

void Form::draw() {
  for (auto mode: drawModes) {
    switch (mode) {
      case DrawMode::Faces: {
        model.drawFaces();
        break;
      }
      
      case DrawMode::Wireframe: {
        drawMesh();
        break;
      }
      
      case DrawMode::Vertices: {
        model.drawVertices();
        break;
      }
      
      case DrawMode::Particles: {
        drawParticles();
      }
      
      default:
        break;
    }
  }
}

void Form::createDynamicParticles() {
  auto meshVertexCount = mesh.getVertices().size();
  // 5 x the actual number of vertices.
  while(dynamicParticles.size() < meshVertexCount * 5) {
    for (int i = 0; i < mesh.getVertices().size(); i++) {
      Particle dp;
      dp.currentPosition = mesh.getVertices()[i];
      dp.velocity = glm::vec3(ofRandom(-0.0005, 0.0001), ofRandom(-0.0003, 0.0003), ofRandom(0.0001, 0.0003));
      dp.radius = 0.0004;
      dp.color = inPlaceParticles[i].color; // Same color as the fixed particles.
      dynamicParticles.push_back(dp);
    }
  }
}

void Form::createInPlaceParticles() {
  inPlaceParticles.clear();
  for (int i = 0; i < mesh.getVertices().size(); i++) {
    Particle p;
    p.currentPosition = mesh.getVertices()[i];
    p.velocity = glm::vec3(0, 0, 0);
    p.radius = 0.001;
    
    auto num = ofRandom(1, 10);
    if (num <= 5) {
      p.color = ofColor::red;
    } else {
      p.color = ofColor::green;
    }
 

    // Add a copy to our inPlaceParticles vector.
    inPlaceParticles.push_back(p);
  }
}

void Form::drawParticles() {
  // Draw the inPlaceParticles.
  ofPushMatrix();
    ofxAssimpMeshHelper &meshHelper = model.getMeshHelper(0);
    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
  
    // In-place particles.
    for (auto &p: inPlaceParticles) {
      p.draw();
    }
  
    // Dynamic particles.
    for (auto &dp: dynamicParticles) {
      dp.draw();
    }
  ofPopMatrix();
}

// Draw openFrameworks mesh.
void Form::drawMesh() {
  #ifndef TARGET_PROGRAMMABLE_GL
  glEnable(GL_NORMALIZE);
  #endif

  // Draw model mesh.
  ofPushMatrix();
    ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);
    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
    mesh.drawWireframe();
  ofPopMatrix();
}

void Form::pushDrawMode(DrawMode mode) {
  drawModes.push_back(mode);
}

void Form::popDrawMode(DrawMode mode) {
  if (drawModes.size() != 0) {
    auto idx = ofFind(drawModes, mode);
    if (idx >= 0 && idx <= drawModes.size()-1) {
      drawModes.erase(drawModes.begin() + idx);
    }
  }
}

int Form::getDynamicParticleCount() {
  return dynamicParticles.size();
}

int Form::getMeshVertexCount() {
  return mesh.getVertices().size();
}
