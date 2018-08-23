#include "Form.h"

void Form::setup(string modelPath) {
  // Initializing constants.
  particleWaitTime = 5000;
  particleSetTime = ofGetElapsedTimeMillis();
  shouldEmitCoins = true;
  
  // Load our model and set it up for animations.
  model.loadModel(modelPath, false);
  model.setPosition(0, 0, 0);
  model.setRotation(0, 180, 0, 0, 1);
  model.setScale(1.5, 1.5, 1.5);
  model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
  model.setPausedForAllAnimations(false);
  model.playAllAnimations();
  
  // Some model smoothing.
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  
  // Load mesh shader.
  meshShader.load("mesh.vert", "mesh.frag");
  
  // Calculate concat matrix.
  concatMatrix.preMult(model.getModelMatrix());
  concatMatrix.preMult(model.getMeshHelper(0).matrix);
  
  // Get the first animated mesh.
  humanMesh = model.getCurrentAnimatedMesh(0);
  
  // Setup shader.
  setupShaderBuffer();
  
  // Create static coins.
  createStaticCoins();
  
  // Initialize camera
  initCamera();
}

void Form::deallocate() {
  // Reset concat matrix to an identity matrix where nothing
  // is transformed at all.
  concatMatrix.makeIdentityMatrix();
  
  // Unload shader.
  coinShader.unload();
  meshShader.unload();
  
  // Clear matrices
  coinMatrices.clear();
  
  // Delete the coin containers.
  for (int i = 0; i < staticCoins.size(); i++) {
    delete staticCoins[i];
  }
  
  for (int i = 0; i < flyingCoins.size(); i++) {
    delete flyingCoins[i];
  }

  staticCoins.clear();
  flyingCoins.clear();
  
  // Clear all draw modes.
  drawModes.clear();
}

void Form::update() {
  // Camera
  cam.update();
  
  // Opacity of the model mesh.
  meshOpacity = ofMap(ofSignedNoise(ofGetElapsedTimef() * 0.2), -1, 1, 0, 0.3, true);
  
  // Update model animation.
  model.update();
  humanMesh = model.getCurrentAnimatedMesh(0);
  
  // Coins.
  updateStaticCoins();
  
  if (shouldEmitCoins) {
    // Create flying coins x seconds after model is created to avoid a sudden burst.
    createFlyingCoins();
    updateFlyingCoins();
  }
}

void Form::draw() {
  cam.begin();
  
    for (auto mode: drawModes) {
      switch (mode) {
        case DrawMode::Faces: {
          model.drawFaces();
          break;
        }
        
        case DrawMode::Wireframe: {
          ofPushStyle();
            ofPushMatrix();
              ofMultMatrix(concatMatrix);
              meshShader.begin();
              meshShader.setUniform3f("uLightPosition", glm::vec3(xMeshLight, yMeshLight, zMeshLight));
              meshShader.setUniform4f("uMaterialColor", ofFloatColor(ofFloatColor::goldenRod));
              meshShader.setUniform1f("uOpacity", meshOpacity);
              meshShader.setUniform1f("uNormalScale", model.getNormalizedScale());
              humanMesh.drawWireframe();
              meshShader.end();
            ofPopMatrix();
          ofPopStyle();
          break;
        }
        
        case DrawMode::Vertices: {
          model.drawVertices();
          break;
        }
        
        case DrawMode::Particles: {
          int size;
          if (staticCoins.size() > 0 && flyingCoins.size() > 0) {
            size = coinMatrices.size();
          } else {
            size = staticCoins.size();
          }
          
          if (staticCoins.size() > 0) {
            coinShader.begin();
              coinShader.setUniform3f("uLightPosition", glm::vec3(xCoinLight, yCoinLight, zCoinLight));
              coinShader.setUniform4f("uMaterialColor", ofColor(ofColor::gold));
              coinMesh.drawInstanced(OF_MESH_FILL, size);
            coinShader.end();
          }
        }
        
        default:
          break;
      }
    }
  
  cam.end();
}

void Form::initCamera() {
  auto sceneCenter = model.getSceneCenter() * model.getNormalizedScale() * model.getScale();
  auto sceneMin = model.getSceneMin() * model.getNormalizedScale() * model.getScale();
  auto sceneMax = model.getSceneMax() * model.getNormalizedScale() * model.getScale();
  auto head = glm::vec3(sceneCenter.x, sceneMax.y, sceneCenter.z);
  cam.init(sceneCenter, sceneMin, sceneMax, head);
}

void Form::setupShaderBuffer() {
  // Load the coin model that'll be instanced.
  coinModel.loadModel("groovecoin3.dae", true);
  coinModel.setPosition(0, 0, 0);
  coinModel.enableTextures();
  coinModel.disableMaterials();
  coinMesh = coinModel.getMesh(0); // Get the coin mesh that'll be instanced.
  coinMesh.setUsage(GL_STATIC_DRAW);
  
  // Setup coin texture to be sent to the shader.
  auto meshHelper = coinModel.getMeshHelper(0);
  auto coinTex = meshHelper.getTextureRef();
  auto coinMatrix = meshHelper.matrix;
  coinTex.generateMipmap();
  coinTex.setTextureMinMagFilter(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
  
  // Max coins are 15 x totalVertices
  maxCoins = humanMesh.getVertices().size() * 11;
  
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
  coinShader.load("coin.vert","coin.frag");
  coinShader.begin();
  coinShader.setUniformMatrix4f("coinMat", coinMatrix);
  coinShader.setUniformTexture("tex",tex,0);
  coinShader.setUniformTexture("coinTex",coinTex,1);
  coinShader.end();
}

void Form::createStaticCoins() {
  // Create a coin at every vertex.
  auto vertices = humanMesh.getVertices();
  for (int i = 0; i < vertices.size(); i++) {
    Coin *c = new Coin;
    auto position = concatMatrix.preMult((ofVec3f) vertices[i]);
    c->setup(true, position);
    c->setPosition(position);
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
  auto elapsedTime = ofGetElapsedTimeMillis() - particleSetTime;
  if (elapsedTime > particleWaitTime && flyingCoins.size() < maxCoins - staticCoins.size()) {
    for (int i = 0; i < staticCoins.size(); i++) {
      Coin *c = new Coin;
      c->setup(false, staticCoins[i]->getPosition());
      // Normal of the current point
      auto normal = humanMesh.getNormal(i);
      c->velocity = glm::normalize(normal) * ofRandom(0.01, 0.05); // For static ofRandom(0.01, 0.05)
      flyingCoins.push_back(c);
    }
    
    // Reset tracking time and also wait time.
    particleSetTime = ofGetElapsedTimeMillis();
    particleWaitTime = ofLerp(particleWaitTime/10000, 3.0, 0.25) * 10000; // Milliseconds.
  }
}

void Form::updateFlyingCoins() {
  for (int i = 0; i < flyingCoins.size(); i++) {
    // Update this coin.
    flyingCoins[i]->update(ofGetLastFrameTime()/ofRandom(10, 100));
    
    // If it's not alive, reset the coin.
    if (!flyingCoins[i] -> isAlive()) {
      // If we don't reset transform, the coin becomes weirdly scaled and bulgy looking.
      flyingCoins[i]->resetTransform();
  
      // Initial position
      int idxForPos = i % staticCoins.size();
      auto startPos = staticCoins[idxForPos]->getPosition();
      
      // Setup again.
      flyingCoins[i]->setup(false, startPos);
      flyingCoins[i]->acceleration = 0.001;

      // Velocity
      auto normal = humanMesh.getNormal(idxForPos);
      flyingCoins[i]->velocity = glm::normalize(normal) * 0.05; // Updated coins fly faster
    }
    
    // Update coinMatrix with the transformation matrix for flying coins.
    coinMatrices[staticCoins.size() + i] = flyingCoins[i]->getLocalTransformMatrix();
  }
  
  // Update buffer with flying coin matrices.
  buffer.updateData(0, coinMatrices);
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

void Form::emitCoins() {
  shouldEmitCoins = !shouldEmitCoins;
}
