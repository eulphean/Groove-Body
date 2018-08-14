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
  
  // Setup material
  coinMaterial.setSpecularColor(ofFloatColor::greenYellow);
  coinMaterial.setDiffuseColor(ofFloatColor::goldenRod);
  coinMaterial.setShininess(15.);
}

void Form::update() {
  // Opacity of the model mesh.
  meshOpacity = ofMap(ofSignedNoise(ofGetElapsedTimef()), -1, 1, 10, 150, true);
  
  // Update model animation.
  model.update();
  mesh = model.getCurrentAnimatedMesh(0);
  
  // Update static coins life.
  auto vertices = mesh.getVertices();
  for (int i = 0; i < vertices.size(); i++) {
    staticCoins[i].update(vertices[i], 0);
  }
  
  // Create flying coins and update their life.
  createFlyingCoins();
  updateFlyingCoins();
}

void Form::initialize() {
  createStaticCoins();
}

void Form::cleanMemory() {
  staticCoins.clear();
  flyingCoins.clear();
  drawModes.clear();
}

void Form::createFlyingCoins() {
  auto vertexCount = mesh.getVertices().size();
  while(flyingCoins.size() < vertexCount * 5) {
    for (int i = 0; i < vertexCount; i++) {
      Coin coin;
      auto coinVel = glm::vec3(ofRandom(-0.0005, 0.0005), ofRandom(-0.0003, 0.0005), ofRandom(0.0007, 0.001));
      coin.setup(coinVel, mesh.getVertices()[i], 0.0004, 0.0001);
      flyingCoins.push_back(coin);
    }
  }
}

void Form::updateFlyingCoins() {
  auto iter = flyingCoins.begin();
  while (iter != flyingCoins.end()) {
    if (iter -> getLife() > 0) {
      float step = ofGetLastFrameTime() / ofRandom(10,15);
      iter -> update(step);
      iter++;
    } else {
      iter = flyingCoins.erase(iter);
    }
  }
}

void Form::createStaticCoins() {
  staticCoins.clear();
  auto vertices = mesh.getVertices();
  for (int i = 0; i < vertices.size(); i++) {
    Coin coin;
    coin.setup(glm::vec3(0, 0, 0), vertices[i], 0.0009, 0.0002);
    staticCoins.push_back(coin);
  }
}

void Form::drawCoins() {
  ofPushMatrix();
    // Matrix transformations
    ofxAssimpMeshHelper &meshHelper = model.getMeshHelper(0);
    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
  
    // Static coins.
    for (auto &p: staticCoins) {
      ofEnableBlendMode(OF_BLENDMODE_SCREEN);
      coinMaterial.begin();
      p.draw();
      coinMaterial.end();
      ofDisableBlendMode();
    }
  
    // Flying coins.
    for (auto &dp: flyingCoins) {
      ofEnableBlendMode(OF_BLENDMODE_ADD);
      coinMaterial.begin();
      dp.draw();
      coinMaterial.end();
      ofDisableBlendMode();
    }
  ofPopMatrix();
}

// Draw openFrameworks mesh.
void Form::drawMesh() {
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
  return flyingCoins.size();
}

int Form::getMeshVertexCount() {
  return mesh.getVertices().size();
}

void Form::draw() {
  for (auto mode: drawModes) {
    switch (mode) {
      case DrawMode::Faces: {
        model.drawFaces();
        break;
      }
      
      case DrawMode::Wireframe: {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPushStyle();
          ofColor c = ofColor(ofColor::goldenRod, meshOpacity);
          ofSetColor(c);
          drawMesh();
        ofPopStyle();
        ofDisableBlendMode();
        break;
      }
      
      case DrawMode::Vertices: {
        model.drawVertices();
        break;
      }
      
      case DrawMode::Particles: {
        drawCoins();
      }
      
      default:
        break;
    }
  }
}
