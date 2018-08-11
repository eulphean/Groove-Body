#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Particle.h"
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
  
    // Helpers
    void drawMesh();
    void pushDrawMode(DrawMode mode);
    void popDrawMode(DrawMode mode);
    void createInPlaceParticles();
    void createDynamicParticles();
    void drawParticles();
    int getDynamicParticleCount();
    int getMeshVertexCount();
    
    // Expose the model to get and set specific properties
    ofxAssimpModelLoader model;
    
  private:
    ofMesh mesh; // of mesh for the model.
    vector<DrawMode> drawModes; 
  
    // Fixed particles around the body.
    vector<Particle> inPlaceParticles;
  
    // Emanating particles.
    vector<Particle> dynamicParticles;
};
