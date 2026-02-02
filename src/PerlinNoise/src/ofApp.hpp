#ifndef OF_APP_HPP
#define OF_APP_HPP


#include "ofMain.h"
#include "ofxGui.h"
#include "ofMesh.h"
#include "perlin2D/perlin2D.hpp"

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
    
    void computeNormals(ofMesh &mesh);
    void keyPressed(int key);
    void keyReleased(int key);
    
    
    
    ofMesh mainMesh;
    ofEasyCam mainCam;
    int width, height;

    

    // gui 
    ofxPanel gui;
    ofxLabel infoLabel;
    ofxIntSlider theta;

private:

    std::unique_ptr<Perlin2D> perlin;
    bool draggingSlider;
    

};


#endif // OF_APP_HPP
