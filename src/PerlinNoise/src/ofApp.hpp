#ifndef OF_APP_HPP
#define OF_APP_HPP



#include "ofMain.h"
#include "perlin2D/perlin2D.hpp"
#include "random"
#include <memory>
#include <fstream>
#include <iostream>



class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void computeNormals(ofMesh &mesh);
    void keyPressed(int key);
    void keyReleased(int key);
    
    
    // make a new mesh object
    ofMesh mainMesh;
    ofEasyCam mainCam;
    int width, height;

private:

    std::unique_ptr<Perlin2D> perlin;
    

};


#endif // OF_APP_HPP