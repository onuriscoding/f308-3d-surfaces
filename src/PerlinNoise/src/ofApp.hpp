#ifndef OF_APP_HPP
#define OF_APP_HPP


#include "ofMain.h"
#include "ofxGui.h"
#include "ofMesh.h"

#include "perlinManager/perlinManager.hpp"

#include <memory>
#include <fstream>
#include <iostream>
#include <string>



class ofApp : public ofBaseApp{
public:
    int currentMap = 1;

    void setup();
    void update();
    void draw();

    void drawPerlin3D();
    void drawPerlin2D();
    void drawSplitView();
    
    void computeNormals(ofMesh &mesh);
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    
public:
    void setParameters(const std::unordered_map<std::string, float>& params);
    
private:
    void loadMap(int mapNumber);
    std::unordered_map<std::string, float> loadParameters(const std::string &filename);
    void saveMap();
    void exportPNG();
    std::string findMapsDir();
    
    ofMesh mesh2D;
    ofMesh mesh3D;
    ofEasyCam mainCam;
    ofEasyCam splitCam2D;
    int width2D, height2D;
    int size3D;
    float spacing;
    std::string mapsDir;

    ofTrueTypeFont guiFont;

    // gui 
    ofxPanel gui;
    ofxLabel infoLabel;
    ofxFloatSlider theta;
    ofxToggle renderPerlin3D;
    ofxToggle movement;
    ofxButton newGeneration;
    ofxFloatSlider perlinScale;
    ofxFloatSlider amplitudeSlider;
    ofxIntSlider octavesSlider;
    ofxFloatSlider sillSlider;
    ofxFloatSlider scale3DSlider;

    ofxIntField seedInput;

    ofxButton saveMapBtn;
    ofxButton exportPngBtn;
    ofxToggle splitView;

    ofxPanel guiPerlin3D;
    ofxToggle rock;
    ofxToggle earth;
    ofxToggle gravel;
    ofxLabel currentMapLabel;
        


    void movementChangedCallBack(bool & value);
    void setValPerlin3DcallBack(bool & value);
    void newGenerationCallback();
    void saveMapCallback();
    void exportPNGCallback();

    void updateMapLabel();

private:

    std::unique_ptr<Perlin2D> perlin2D;
    
    std::unique_ptr<PerlinManager> perlinManager;

    
    bool draggingThetaSlider;
    bool dragingScaleSlider;
    bool draggingAmplitudeSlider;
    bool draggingOctavesSlider;
    bool draggingSillSlider;
    bool draggingScale3DSlider;
    bool editingSeed = false;


    void initMesh2D();
    void initMesh3D();
    

};


#endif // OF_APP_HPP