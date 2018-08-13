#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Coin.h"
#include "ofxGui.h"

enum DrawMode {
  Wireframe,
  Faces,
  Vertices,
  Particles
};

class Form {
  public:
    // Constructor
    Form(string model);
    void update();
    void draw();
  
    // Pubic methods.
    void pushDrawMode(DrawMode mode);
    void popDrawMode(DrawMode mode);
    int getDynamicParticleCount();
    int getMeshVertexCount();
    
    // Expose the model to get and set specific properties
    ofxAssimpModelLoader model;
    
  private:
    ofMesh mesh;
    vector<DrawMode> drawModes;
    // Fixed coins that make up the body.
    vector<Coin> staticCoins;
    // Flying coins.
    vector<Coin> flyingCoins;
    float meshOpacity;
  
    // Private methods.
    void drawMesh();
    void drawCoins();
    void createStaticCoins();
    void createFlyingCoins();
    void updateFlyingCoins();
};
