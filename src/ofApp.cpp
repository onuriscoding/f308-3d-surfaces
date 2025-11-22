#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    
    cam.setDistance(200);
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    int gridSize = 20;
    float spacing = 10.0f;
    
    // Génération d'une surface de test
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            float xPos = (x - gridSize/2) * spacing;
            float yPos = (y - gridSize/2) * spacing;
            float zPos = 0; 
            
            mesh.addVertex(ofVec3f(xPos, yPos, zPos));
            mesh.addColor(ofFloatColor(x / (float)gridSize, y / (float)gridSize, 0.5));
        }
    }
    
    
    for (int y = 0; y < gridSize - 1; y++) {
        for (int x = 0; x < gridSize - 1; x++) {
            int i = x + y * gridSize;
            
            mesh.addIndex(i);
            mesh.addIndex(i + 1);
            mesh.addIndex(i + gridSize);
            
            mesh.addIndex(i + 1);
            mesh.addIndex(i + gridSize + 1);
            mesh.addIndex(i + gridSize);
        }
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    
    ofPushMatrix();
    
    
    ofSetColor(255);
    mesh.draw();
    
    
    ofSetColor(100);
    mesh.drawWireframe();
    
    ofPopMatrix();
    
    cam.end();
    
    
    ofSetColor(255);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 20, 20);
    ofDrawBitmapString("Projet 3D Surfaces - F308", 20, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
    
}
