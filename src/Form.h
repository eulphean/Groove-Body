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
    ofParameter<float> xLight { "x: light", 0, -5000, 5000 };
    ofParameter<float> yLight { "y: light", 500, -5000, 5000 };
    ofParameter<float> zLight { "z: light", 500, 0, 10000 };
  private:
    BreathCam cam;
    
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
  
    ofMaterial material;
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
  
    // Light (only for Mesh, use the position for global light).
    ofLight light;

};
