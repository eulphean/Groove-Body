#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Coin.h"

enum DrawMode {
  Wireframe,
  Faces,
  Vertices,
  Particles
};

class Form {
  public:
    void update();
    void draw();
  
    // Pubic methods.
    void pushDrawMode(DrawMode mode);
    void popDrawMode(DrawMode mode);
    int getDynamicParticleCount();
    int getMeshVertexCount();
  
    // Important methods. 
    void setup(string modelPath);
    void deallocate();
    
    // Expose the model to get and set specific properties
    ofxAssimpModelLoader model;
    bool updateCameraState = false;
    
  private:
    ofMatrix4x4 concatMatrix; 
    ofMesh humanMesh;
    ofVboMesh cylinderMesh;
    vector<Coin*> staticCoins;
    vector<Coin*> flyingCoins;
  
    // Coin drawing shader.
    ofShader shader;
    ofBufferObject buffer;
    vector<ofMatrix4x4> coinMatrices;
    ofTexture tex;
  
    float meshOpacity;
    vector<DrawMode> drawModes;
  
    // Shader
    void setupShaderBuffer();
  
    // Static coins.
    void createStaticCoins();
    void updateStaticCoins();
  
    // Flying coins.
    void createFlyingCoins();
    void updateFlyingCoins();
  
    int maxCoins; // Static + Dynamic coins.
    unsigned int initTime; 
};
