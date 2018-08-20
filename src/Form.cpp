#include "Form.h"

void Form::setup(string modelPath) {
  // Load the coin model.
  coinModel.loadModel("coin.dae", true);
  coinModel.setPosition(0, 0, 0);
  coinModel.enableTextures();
  coinModel.disableMaterials();
  coinMesh = coinModel.getMesh(0); // Get the coin mesh that'll be instanced.
  
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
  
  // Store initialize time.
  initTime = ofGetElapsedTimeMillis();
  
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
  // Create flying coins x seconds after model is created to avoid a sudden burst.
  if (ofGetElapsedTimeMillis() - initTime > 5000) {
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
              coinShader.setUniform4f("uMaterialColor", ofColor(ofColor::yellowGreen));
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
  // Max coins are 15 x totalVertices
  maxCoins = humanMesh.getVertices().size() * 12;
  
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

  // Coin texture.
  auto coinTex = coinModel.getMeshHelper(0).getTextureRef();
  coinTex.generateMipmap();
  coinTex.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
  
  // Set texture uniform in the shader. 
  coinShader.load("coin.vert","coin.frag");
  coinShader.begin();
  coinShader.setUniformTexture("tex",tex,0);
  coinShader.setUniformTexture("coinTex",coinTex,1);
  coinShader.end();

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
    c->setScale(8.0);
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
  // Only executes once and populates the flyingCoins vector.
  while (flyingCoins.size() < maxCoins - staticCoins.size()) {
    for (int i = 0; i < staticCoins.size(); i++) {
      Coin *c = new Coin;
      c->setPosition(staticCoins[i]->getPosition());
      c->setScale(7.0);
      // Initial velocity for first flying coin creation.
      c->velocity = glm::vec3(ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-1, 1));
      flyingCoins.push_back(c);
    }
  }
}

void Form::updateFlyingCoins() {
  for (int i = 0; i < flyingCoins.size(); i++) {
    // Update this coin.
    flyingCoins[i]->update(ofGetLastFrameTime()/ofRandom(1, 150));
    
    // If it's not alive, reset the coin.
    if (!flyingCoins[i] -> isAlive()) {
      // Update coin at a position.
      int idxForPos = i % staticCoins.size();
      flyingCoins[i]->setPosition(staticCoins[idxForPos]->getPosition());
      flyingCoins[i]->velocity = glm::vec3(ofRandom(-2, 2), ofRandom(-2, 2), ofRandom(-1, 1));
      flyingCoins[i]->life = 1.0;
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
