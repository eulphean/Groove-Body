#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Coin.h"
#include "CameraProfile.h"

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
  
    // Lighting.
  
    // Coins
    ofParameter<float> xCoinLight { "x: Coin light", 0, -5000, 5000 };
    ofParameter<float> yCoinLight { "y: Coing light", 500, -5000, 5000 };
    ofParameter<float> zCoinLight { "z: Coin light", 500, 0, 10000 };
  
    // Human mesh. 
    ofParameter<float> xMeshLight { "x: Mesh ight", 0, -5000, 5000 };
    ofParameter<float> yMeshLight { "y: Mesh light", 500, -5000, 5000 };
    ofParameter<float> zMeshLight { "z: Mesh light", 500, 0, 10000 };
  private:
    BreathCam cam;
    
    ofMatrix4x4 concatMatrix;
    ofMesh humanMesh;
    ofShader meshShader;
  
    ofVboMesh cylinderMesh;
    vector<Coin*> staticCoins;
    vector<Coin*> flyingCoins;
  
    // Coin drawing shader.
    ofShader coinShader;
    ofBufferObject buffer;
    vector<ofMatrix4x4> coinMatrices;
    ofTexture tex;
  
    float meshOpacity;
    vector<DrawMode> drawModes;
  
    int maxCoins; // Static + Dynamic coins.
    float initTime; 
  
    // Shader
    void setupShaderBuffer();
  
    // Static coins.
    void createStaticCoins();
    void updateStaticCoins();
  
    // Flying coins.
    void createFlyingCoins();
    void updateFlyingCoins();
  
    // Camera
    void initCamera();
};
