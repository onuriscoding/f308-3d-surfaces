#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BezierMath.h"

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
    
    void calculateBezierCurve();
    void updateControlPoints(int newCount);
    void drawEquations();
    void drawBernsteinBasis();
    void drawStats();
    void calculateStats();
    
    // Statistiques
    float curveLength;
    float minHeight, maxHeight;
    vector<float> tangents;
    vector<float> curvatures;
    
    // GUI
    ofxPanel gui;
    ofxIntSlider numPointsSlider;
    ofxIntSlider numControlPointsSlider;
    ofxToggle showControlPoints;
    ofxToggle showControlLines;
    ofxToggle showBernsteinBasis;
    ofxToggle showStats;
    ofxFloatSlider curveThickness;
    
    const int MIN_CONTROL_POINTS = 2;
    const int MAX_CONTROL_POINTS = 10;
};