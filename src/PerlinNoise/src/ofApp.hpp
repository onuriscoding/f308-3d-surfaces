#ifndef OF_APP_HPP
#define OF_APP_HPP


#include "ofMain.h"
#include "ofxGui.h"
#include "ofMesh.h"

#include "perlin3D/perlin3D.hpp"

#include "random"
#include <memory>
#include <fstream>
#include <iostream>
#include <string>



class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void drawPerlin3D();
    void drawPerlin2D();
    
    void computeNormals(ofMesh &mesh);
    void keyPressed(int key);
    void keyReleased(int key);
    
    
    
    ofMesh mesh2D;
    ofMesh mesh3D;
    ofEasyCam mainCam;
    int width2D, height2D;
    int size3D;
    float spacing;

    

    // gui 
    ofxPanel gui;
    ofxLabel infoLabel;
    ofxFloatSlider theta;
    ofxToggle renderPerlin3D;
    ofxToggle movement;
    ofxButton newGeneration;
    ofxFloatSlider perlinScale;

    


    void movementChangedCallBack(bool & value);

private:

    std::unique_ptr<Perlin2D> perlin2D;
    std::unique_ptr<Perlin3D> perlin3D;
    bool draggingThetaSlider;
    bool dragingScaleSlider;


    void initMesh2D();
    void initMesh3D();
    

};


#endif // OF_APP_HPP
