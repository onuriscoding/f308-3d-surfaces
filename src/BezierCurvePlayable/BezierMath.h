#pragma once

#include "ofMain.h"

class BezierMath {
public:
    // Point sur une courbe de Bézier de degré n
    // t \in [0,1] : paramètre de la courbe (0 = début, 1 = fin)
    static ofVec2f calculatePoint(float t, const vector<ofVec2f>& controlPoints);
    
    static int binomialCoefficient(int n, int i);
    
    static float bernsteinPolynomial(int i, int n, float t);
    
    static string getCurveName(int numPoints);
    
    static string getEquation(int numPoints);
};