#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BezierMath.h"
#include "ofMesh.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    vector<ofVec2f> controlPoints;
    vector<ofVec2f> bezierPoints;
    int selectedPoint;
    int numSteps;
    int numControlPoints;
    int deltaRandom;
    ofMesh riverMesh;
    
    void calculateBezierCurve();
    void calculateRiver();
    void updateControlPoints(int newCount);
    void drawEquations();
    void drawBernsteinBasis();
    void drawStats();
    void calculateStats();
    void drawRiver();
    void genRandomisedCurve();
    
    // Statistiques
    float curveLength;
    float minHeight, maxHeight;
    vector<float> tangents;
    vector<float> curvatures;
    
    // GUI
    ofxPanel gui;
    ofxIntSlider numPointsSlider;
    ofxIntSlider numControlPointsSlider;
    ofxIntSlider deltaRandomSlider;
    ofxToggle showControlPoints;
    ofxToggle showControlLines;
    ofxToggle showBernsteinBasis;
    ofxToggle showStats;
    ofxFloatSlider curveThickness;
    ofxToggle riverMode;
    ofxToggle coloredRiver;
    ofxButton randomCurveAtoB;
    
    const int MIN_CONTROL_POINTS = 2;
    const int MAX_CONTROL_POINTS = 10;
    float riverWidth = 20.0f;
};