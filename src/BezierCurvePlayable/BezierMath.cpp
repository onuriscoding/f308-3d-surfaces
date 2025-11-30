#include "BezierMath.h"
#include <cmath>


ofVec2f BezierMath::calculatePoint(float t, const vector<ofVec2f>& controlPoints) {
    int n = controlPoints.size() - 1; // Degré de la courbe
    ofVec2f point(0, 0);
    
    // Formule générale : B(t) = Σ(i=0 to n) B_{i,n}(t) * P_i
    // avec B_{i,n}(t) le [polynôme de Bernstein](https://en.wikipedia.org/wiki/Bernstein_polynomial)
    for (int i = 0; i <= n; i++) {
        float basis = bernsteinPolynomial(i, n, t);
        point += basis * controlPoints[i];
    }
    
    return point;
}


int BezierMath::binomialCoefficient(int n, int i) {
    // via [Math Stack Exchange](https://math.stackexchange.com/a/927064)
    if (i > n) return 0;
    if (i == 0 || i == n) return 1;
    
    int result = 1;
    for (int k = 1; k <= i; k++) {
        result *= (n - k + 1);
        result /= k;
    }
    return result;
}


float BezierMath::bernsteinPolynomial(int i, int n, float t) {
    // B_{i,n}(t) = C(n,i) * t^i * (1-t)^(n-i)
    float coeff = binomialCoefficient(n, i);
    float ti = pow(t, i);
    float tni = pow(1 - t, n - i);
    
    return coeff * ti * tni;
}


string BezierMath::getCurveName(int numPoints) {
    switch(numPoints) {
        case 2: return "Linéaire (degré 1)";
        case 3: return "Quadratique (degré 2)";
        case 4: return "Cubique (degré 3)";
        case 5: return "Quartique (degré 4)";
        case 6: return "Quintique (degré 5)";
        default: return "Bézier de degré " + ofToString(numPoints - 1);
    }
}


string BezierMath::getEquation(int numPoints) {
    int n = numPoints - 1;
    string eq = "B(t) = ";
    
    for (int i = 0; i <= n; i++) {
        if (i > 0) eq += " + ";
        
        int coeff = binomialCoefficient(n, i);
        if (coeff > 1) eq += ofToString(coeff);
        
        // t^i
        if (i > 0) {
            eq += "t";
            if (i > 1) eq += "^" + ofToString(i);
        }
        
        // (1-t)^(n-i)
        if (n - i > 0) {
            eq += "(1-t)";
            if (n - i > 1) eq += "^" + ofToString(n - i);
        }
        
        // Control point
        eq += "P" + ofToString(i);
    }
    
    return eq;
}