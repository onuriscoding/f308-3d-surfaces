#include "ofApp.h"
#include <algorithm>


void ofApp::setup(){
    ofSetWindowTitle("Courbes de Bézier - Générateur de Terrain");
    ofSetFrameRate(60);
    ofBackground(30);
    
    numControlPoints = 4;
    selectedPoint = -1;
    numSteps = 100;
    curveLength = 0;
    deltaRandom = 1;
    riverMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    // Initialiser les points de contrôle
    updateControlPoints(numControlPoints);
    
    // Setup GUI
    gui.setup("Paramètres");
    gui.add(numControlPointsSlider.setup("Nb points contrôle", 4, MIN_CONTROL_POINTS, MAX_CONTROL_POINTS));
    gui.add(numPointsSlider.setup("Résolution courbe", 100, 10, 500));
    gui.add(curveThickness.setup("Épaisseur", 2, 1, 10));
    gui.add(deltaRandomSlider.setup("Delta Random", 2, 1, 10));
    gui.add(showControlPoints.setup("Points contrôle", true));
    gui.add(showControlLines.setup("Lignes contrôle", true));
    gui.add(showBernsteinBasis.setup("Base Bernstein", false));
    gui.add(showStats.setup("Statistiques", true));
    gui.add(riverMode.setup("River Mode", false));
    gui.add(coloredRiver.setup("Colored Mode", false));
    gui.add(randomCurveAtoB.setup("Generate Random River Between A and B"));
}


void ofApp::update(){
    numSteps = numPointsSlider;
    deltaRandom = deltaRandomSlider;
    
    // Si le nombre de points change
    if (numControlPoints != (int)numControlPointsSlider) {
        updateControlPoints(numControlPointsSlider);
    }
    if (randomCurveAtoB) {
        genRandomisedCurve();
    }
    calculateBezierCurve();
    calculateRiver();
    calculateStats();
}


void ofApp::updateControlPoints(int newCount) {
    vector<ofVec2f> oldPoints = controlPoints;
    numControlPoints = newCount;
    controlPoints.clear();
    controlPoints.resize(numControlPoints);
    
    float width = ofGetWidth() - 250;
    float height = ofGetHeight() - 250;
    float startX = 150;
    float centerY = ofGetHeight() / 2;
    
    // points initialement "uniformes" (~sinus)
    for (int i = 0; i < numControlPoints; i++) {
        if (i < oldPoints.size()) {
            controlPoints[i] = oldPoints[i];
        } else {
            float t = (float)i / (numControlPoints - 1);
            float x = startX + t * width;
            float y = centerY + sin(t * PI * 2) * height * 0.25;
            controlPoints[i] = ofVec2f(x, y);
        }
    }
}

void ofApp::draw(){
    if (showControlLines) {
        ofSetColor(100, 100, 100);
        ofSetLineWidth(1);
        for (int i = 0; i < controlPoints.size() - 1; i++) {
            ofDrawLine(controlPoints[i], controlPoints[i + 1]);
        }
    }

    drawRiver();
    
    // Courbe de Bézier
    ofSetColor(0, 200, 255);
    ofSetLineWidth(curveThickness);
    ofNoFill();
    ofBeginShape();
    for (auto& point : bezierPoints) {
        ofVertex(point.x, point.y);
    }
    ofEndShape();
    ofFill();
    
    if (showControlPoints) {
        for (int i = 0; i < controlPoints.size(); i++) {
            if (i == selectedPoint) {
                ofSetColor(255, 100, 100);
                ofDrawCircle(controlPoints[i], 10);
            } else {
                ofSetColor(255, 200, 0);
                ofDrawCircle(controlPoints[i], 8);
            }
            
            // # du point
            ofSetColor(255);
            ofDrawBitmapString("P" + ofToString(i), 
                             controlPoints[i].x + 12, 
                             controlPoints[i].y + 5);
        }
    }
    
    // Cbase Bernstein
    if (showBernsteinBasis) {
        drawBernsteinBasis();
    }

    if (showStats) {
        drawStats();
    }
    
    drawEquations();
    gui.draw();
}


void ofApp::calculateBezierCurve() {
    bezierPoints.clear();
    
    for (int i = 0; i <= numSteps; i++) {
        float t = (float)i / numSteps;
        ofVec2f point = BezierMath::calculatePoint(t, controlPoints);
        bezierPoints.push_back(point);
    }
}

void ofApp::calculateRiver() {
    if (riverMode) { // (c'est possible de l'inclure dans le for précedent)
        riverMesh.clear();
        riverMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

        for (int i = 1; i <= numSteps; i++) {

            ofVec2f tangente = bezierPoints[i] - bezierPoints[i-1]; // vecteur direction entre 2 points
            tangente.normalize(); // norme = 1
            ofVec2f normale = tangente.rotate(90);

            ofVec2f left  = bezierPoints[i] + normale * riverWidth;
            ofVec2f right = bezierPoints[i] - normale *  riverWidth;

            riverMesh.addVertex(ofVec3f(left.x, left.y, 0));
            riverMesh.addVertex(ofVec3f(right.x, right.y, 0));
        }
    }
}


void ofApp::calculateStats() {
    if (bezierPoints.size() < 2) return;
    
    curveLength = 0;
    minHeight = bezierPoints[0].y;
    maxHeight = bezierPoints[0].y;
    
    for (int i = 1; i < bezierPoints.size(); i++) {
        float dist = bezierPoints[i].distance(bezierPoints[i-1]);
        curveLength += dist;
        
        minHeight = std::min(minHeight, bezierPoints[i].y);
        maxHeight = std::max(maxHeight, bezierPoints[i].y);
    }
    
    // Calculer les tangentes et courbures
    tangents.clear();
    curvatures.clear();
    
    for (int i = 0; i < bezierPoints.size(); i++) {
        float t = (float)i / (bezierPoints.size() - 1);
        
        // Dérivée première (tangente) par [différences finies](https://en.wikipedia.org/wiki/Finite_difference)
        ofVec2f tangent;
        if (i == 0) {
            tangent = bezierPoints[1] - bezierPoints[0];
        } else if (i == bezierPoints.size() - 1) {
            tangent = bezierPoints[i] - bezierPoints[i-1];
        } else {
            tangent = (bezierPoints[i+1] - bezierPoints[i-1]) * 0.5;
        }
        
        float angle = atan2(tangent.y, tangent.x) * RAD_TO_DEG;
        tangents.push_back(angle);
        
        // Courbure approx.
        if (i > 0 && i < bezierPoints.size() - 1) {
            ofVec2f p0 = bezierPoints[i-1];
            ofVec2f p1 = bezierPoints[i];
            ofVec2f p2 = bezierPoints[i+1];
            
            float area = abs((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
            float a = p0.distance(p1);
            float b = p1.distance(p2);
            float c = p2.distance(p0);
            
            if (a * b * c > 0) {
                float k = 4 * area / (a * b * c);
                curvatures.push_back(k);
            } else {
                curvatures.push_back(0);
            }
        }
    }
}


void ofApp::drawStats() {
    int x = ofGetWidth() - 280;
    int y = ofGetHeight() - 250;
    int w = 260;
    int h = 230;
    
    // Fond 
    ofSetColor(40, 40, 40, 220);
    ofDrawRectangle(x, y, w, h);
    
    // Bordure
    ofSetColor(100, 200, 255);
    ofNoFill();
    ofSetLineWidth(2);
    ofDrawRectangle(x, y, w, h);
    ofFill();
    
    // Titre
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("STATS", x + 10, y + 20);
    
    y += 35;
    ofSetColor(200, 200, 200);
    
    // Type de courbe
    string curveName = BezierMath::getCurveName(controlPoints.size());
    ofDrawBitmapString("Type: " + curveName, x + 10, y);
    y += 20;
    
    // Longueur de la courbe
    ofDrawBitmapString("Longueur: " + ofToString(curveLength, 1) + " px", x + 10, y);
    y += 20;
    
    // Hauteur min/max
    float heightRange = maxHeight - minHeight;
    ofDrawBitmapString("Dénivelé: " + ofToString(heightRange, 1) + " px", x + 10, y);
    y += 15;
    ofSetColor(150, 150, 150);
    ofDrawBitmapString("  Min: " + ofToString(minHeight, 1), x + 15, y);
    y += 15;
    ofDrawBitmapString("  Max: " + ofToString(maxHeight, 1), x + 15, y);
    y += 20;
    
    // Courbure moyenne
    if (curvatures.size() > 0) {
        float avgCurvature = 0;
        float maxCurvature = 0;
        for (float k : curvatures) {
            avgCurvature += k;
            maxCurvature = max(maxCurvature, k);
        }
        avgCurvature /= curvatures.size();
        
        ofSetColor(200, 200, 200);
        ofDrawBitmapString("Courbure moy: " + ofToString(avgCurvature, 4), x + 10, y);
        y += 15;
        ofSetColor(150, 150, 150);
        ofDrawBitmapString("  Max: " + ofToString(maxCurvature, 4), x + 15, y);
        y += 20;
    }
    
    // Info pour terrain 3D
    ofSetColor(100, 200, 255);
    ofDrawBitmapString("UTILISATION TERRAIN:", x + 10, y);
    y += 15;
    ofSetColor(150, 200, 150);
    ofDrawBitmapString("- Dénivelé = variation altitude", x + 15, y);
    y += 15;
    ofDrawBitmapString("- Courbure = pente du terrain", x + 15, y);
    y += 15;
    ofDrawBitmapString("- Longueur = distance parcourue", x + 15, y);
}

void ofApp::drawRiver() {
    if (riverMode) {
        ofSetColor(0, 120, 255, 150);
        if (not coloredRiver) riverMesh.drawWireframe();
        else riverMesh.draw();
    }
}

void ofApp::genRandomisedCurve() {
    ofVec2f A = controlPoints[0];
    ofVec2f B = controlPoints[controlPoints.size()-1];
    vector<ofVec2f> randomControlPoints; // Points de la rivière randomisé courbe de bézier

    ofVec2f AB = (B-A);
    ofVec2f segment = AB/(numControlPoints+1); // aToB divisé en numRandomRiverPoints segments

    randomControlPoints.push_back(A);
    for (int i = 1; i<numControlPoints+1; i++) {
        ofVec2f randomPoint = A + segment*i;
        ofVec2f normalVector = segment.getRotated(90).getNormalized();

        float maxDist = deltaRandom * AB.length() * 1/(numControlPoints+1);
        float randomDist = ofRandom(-maxDist, maxDist);

        randomPoint += randomDist*normalVector;
        randomControlPoints.push_back(randomPoint);
    }
    randomControlPoints.push_back(B);

    for (int i = 0; i <= numSteps; i++) {
        float t = (float)i / numSteps;
        ofVec2f point = BezierMath::calculatePoint(t, randomControlPoints);
        bezierPoints.push_back(point);
    }
    controlPoints = randomControlPoints;
}


void ofApp::drawEquations() {
    ofSetColor(200);
    int x = 20;
    int y = ofGetHeight() - 120;
    
    // Nom de la courbe
    string curveName = BezierMath::getCurveName(controlPoints.size());
    ofDrawBitmapString("COURBE DE BÉZIER " + curveName, x, y);
    y += 20;
    
    // Éq. générale
    string equation = BezierMath::getEquation(controlPoints.size());
    ofDrawBitmapString(equation, x, y);
    y += 20;
    
    ofSetColor(150);
    ofDrawBitmapString("où t in [0, 1]  (t=0: début, t=1: fin)", x, y);
    y += 20;
    
    ofDrawBitmapString("Polynômes de Bernstein: B_{i,n}(t) = C(n,i) * t^i * (1-t)^(n-i)", x, y);
}


void ofApp::drawBernsteinBasis() {
    int graphX = ofGetWidth() - 320;
    int graphY = 20;
    int graphW = 300;
    int graphH = 200;
    
    // Fond
    ofSetColor(40, 40, 40, 220);
    ofDrawRectangle(graphX, graphY, graphW, graphH);
    
    // Bordure
    ofSetColor(100, 200, 255);
    ofNoFill();
    ofSetLineWidth(2);
    ofDrawRectangle(graphX, graphY, graphW, graphH);
    ofFill();
    
    // Titre
    ofSetColor(255);
    ofDrawBitmapString("BASE DE BERNSTEIN", graphX + 10, graphY + 15);
    
    // Axes
    ofSetColor(80, 80, 80);
    ofSetLineWidth(1);
    ofDrawLine(graphX, graphY + graphH, graphX + graphW, graphY + graphH); // X
    ofDrawLine(graphX, graphY, graphX, graphY + graphH); // Y
    
    // Graduations
    ofSetColor(60, 60, 60);
    for (int i = 0; i <= 4; i++) {
        float yPos = graphY + graphH - (i * 0.25 * graphH);
        ofDrawLine(graphX, yPos, graphX + graphW, yPos);
    }
    
    // Dessiner chaque polynôme de Bernstein
    int n = controlPoints.size() - 1;
    vector<ofColor> colors = {
        ofColor(255, 100, 100),
        ofColor(100, 255, 100),
        ofColor(100, 100, 255),
        ofColor(255, 255, 100),
        ofColor(255, 100, 255),
        ofColor(100, 255, 255),
        ofColor(255, 180, 100),
        ofColor(180, 100, 255),
        ofColor(100, 255, 180),
        ofColor(255, 100, 180)
    };
    
    ofSetLineWidth(2);
    for (int i = 0; i <= n; i++) {
        ofSetColor(colors[i % colors.size()]);
        ofNoFill();
        ofBeginShape();
        for (int step = 0; step <= 100; step++) {
            float t = step / 100.0f;
            float basis = BezierMath::bernsteinPolynomial(i, n, t);
            float x = graphX + t * graphW;
            float y = graphY + graphH - basis * graphH;
            ofVertex(x, y);
        }
        ofEndShape();
        ofFill();
    }
    
    // Légende
    ofSetColor(200);
    int legendY = graphY + 35;
    for (int i = 0; i <= n; i++) {
        ofSetColor(colors[i % colors.size()]);
        ofDrawCircle(graphX + 10, legendY + i * 15, 3);
        ofDrawBitmapString("B" + ofToString(i) + "," + ofToString(n), graphX + 20, legendY + i * 15 + 5);
    }
}


void ofApp::mousePressed(int x, int y, int button){
    const int mouse_tolerance_radius = 20;
    for (int i = 0; i < controlPoints.size(); i++) {
        float dist = controlPoints[i].distance(ofVec2f(x, y));
        if (dist < mouse_tolerance_radius) {
            selectedPoint = i;
            return;
        }
    }
    selectedPoint = -1;
}


void ofApp::mouseDragged(int x, int y, int button){
    if (selectedPoint >= 0) {
        controlPoints[selectedPoint].set(x, y);
    }
}


void ofApp::mouseReleased(int x, int y, int button){
    selectedPoint = -1;
}


void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
    else if (key == '+' || key == '=') {
        if (numControlPoints < MAX_CONTROL_POINTS) {
            numControlPointsSlider = numControlPoints + 1;
        }
    }
    else if (key == '-') {
        if (numControlPoints > MIN_CONTROL_POINTS) {
            numControlPointsSlider = numControlPoints - 1;
        }
    }
}