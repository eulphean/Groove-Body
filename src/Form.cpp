#include "Form.h"

void Form::setup(string modelPath) {
  // Load our model and set it up for animations.
  model.loadModel(modelPath, false);
  model.setPosition(0, 0, 0);
  model.setRotation(0, 180, 0, 0, 1);
  model.setScale(1.5, 1.5, 1.5);
  model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
  model.setPausedForAllAnimations(false);
  model.playAllAnimations();
  
  // Calculate concat matrix.
  concatMatrix.preMult(model.getModelMatrix());
  concatMatrix.preMult(model.getMeshHelper(0).matrix);
  
  // Get the first animated mesh.
  humanMesh = model.getCurrentAnimatedMesh(0);
  
  // Setup shader.
  setupShaderBuffer();
  
  // Create static coins.
  createStaticCoins();
}

void Form::deallocate() {
  // Reset concat matrix to an identity matrix where nothing
  // is transformed at all.
  concatMatrix.makeIdentityMatrix();
  
  // Unload shader.
  shader.unload();
  
  // Clear matrices
  coinMatrices.clear();
  
  // Delete the coin containers.
  for (int i = 0; i < staticCoins.size(); i++) {
    delete staticCoins[i];
  }

  staticCoins.clear();
  
  flyingCoins.clear();
  
  // Clear all draw modes.
  drawModes.clear();
}

void Form::update() {
  // Opacity of the model mesh.
  meshOpacity = ofMap(ofSignedNoise(ofGetElapsedTimef()), -1, 1, 0, 100, true);
  
  // Update model animation.
  model.update();
  humanMesh = model.getCurrentAnimatedMesh(0);
  
  updateStaticCoins();
  
  // Create flying coins and update their life.
  //createFlyingCoins();
  //updateFlyingCoins();
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
          auto c = ofColor(ofColor::darkGoldenRod, meshOpacity);
          ofSetColor(c);
          ofPushMatrix();
            ofMultMatrix(concatMatrix);
            humanMesh.drawWireframe();
          ofPopMatrix();
        ofPopStyle();
        ofDisableBlendMode();
        break;
      }
      
      case DrawMode::Vertices: {
        model.drawVertices();
        break;
      }
      
      case DrawMode::Particles: {
        if (staticCoins.size() > 0 || flyingCoins.size() > 0) {
          shader.begin();
            cylinderMesh.drawInstanced(OF_MESH_FILL, coinMatrices.size());
          shader.end();
        }
      }
      
      default:
        break;
    }
  }
}

void Form::setupShaderBuffer() {
  // Max number of coin transformations this matrix will hold.
  // This is static + flying coins.
  coinMatrices.resize(maxCoins);
  
  // Allocate buffer.
  buffer.allocate();
  buffer.bind(GL_TEXTURE_BUFFER);
  buffer.setData(coinMatrices,GL_STREAM_DRAW);

  // Allocate as buffer texture. GL_RGBA32F allows us to get
  // each row of the matrix as a vec4.
  tex.allocateAsBufferTexture(buffer,GL_RGBA32F);

  // Set texture uniform in the shader. 
  shader.load("vert.glsl","frag.glsl");
  shader.begin();
  shader.setUniformTexture("tex",tex,0);
  shader.end();

  // Set the geometry which will be drawn as an instance drawing.
  ofCylinderPrimitive cylinder;
  cylinder.set(5, 1);
  cylinder.setResolution(15, 10);
  cylinder.setCylinderColor(ofColor::darkGoldenRod);
  cylinder.setTopCapColor(ofColor::gold);
  cylinder.setBottomCapColor(ofColor::gold);
  cylinderMesh = cylinder.getMesh();
  cylinderMesh.setUsage(GL_STATIC_DRAW);
}

void Form::createStaticCoins() {
  // Create a coin at every vertex.
  auto vertices = humanMesh.getVertices();
  for (int i = 0; i < vertices.size(); i++) {
    Coin *c = new Coin;
    auto position = concatMatrix.preMult((ofVec3f) vertices[i]);
    c->setPosition(position); // position.
    c->velocity = glm::vec3(0, 0, 0); // static.
    staticCoins.push_back(c);
  }
}

void Form::updateStaticCoins() {
  auto vertices = humanMesh.getVertices();
  for (int i = 0; i < vertices.size(); i++) {
    // Animated position.
    auto position = concatMatrix.preMult((ofVec3f) vertices[i]);
    
    staticCoins[i]->setPosition(position);
    staticCoins[i]->update(0); // Call update for rotation.
    
    // Set transformation matrix for this coin.
    coinMatrices[i] = staticCoins[i]->getLocalTransformMatrix();
  }
  
  buffer.updateData(0, coinMatrices);
}

void Form::createFlyingCoins() {
  
}

void Form::updateFlyingCoins() {
  
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
  return humanMesh.getVertices().size();
}
