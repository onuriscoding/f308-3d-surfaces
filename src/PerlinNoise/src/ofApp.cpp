#include "ofApp.hpp"




void ofApp::setup(){
    perlin = std::make_unique<Perlin2D>(0.02,250, 13.0);
    
    mainMesh.setMode(OF_PRIMITIVE_TRIANGLES);

    
    
    ofEnableDepthTest();
    
    //set the width and height for our mesh and initial rendering values
    width = 500;
    height = 500;
    ofBackground(0);
    ofSetColor(255);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            float cx = x - width  * 0.5f;
            float cy = y - height * 0.5f;
            mainMesh.addVertex(glm::vec3(cx, cy, 0));
            mainMesh.addColor(ofFloatColor(1.0)); 
        }
    }

    
    for(int y = 0; y < height - 1; y++){
        for(int x = 0; x < width - 1; x++){

            int i1 =  x      +  y      * width;
            int i2 = (x + 1) +  y      * width;
            int i3 =  x      + (y + 1) * width;
            int i4 = (x + 1) + (y + 1) * width;            
            mainMesh.addIndex(i1);
            mainMesh.addIndex(i2);
            mainMesh.addIndex(i3);
            mainMesh.addIndex(i2);
            mainMesh.addIndex(i4);
            mainMesh.addIndex(i3);
        }
    }

  

    
    mainCam.setPosition(0, 0, 600);
    mainCam.lookAt(glm::vec3(0,0,0));


}

//--------------------------------------------------------------
void ofApp::update(){
    perlin->updateMesh(mainMesh, height, width );
    
    // for (int y = 0 ; y < height; y ++){
    //     for (int x = 0 ; x < width; x ++){
    //         int index = x + y * width;
    //         ofVec3f v = mainMesh.getVertex(index);


    //     }
    // }
    //computeNormals(mainMesh);
    
}




//--------------------------------------------------------------
void ofApp::draw(){
    
    glPointSize(5);
    mainCam.begin();
    mainMesh.draw();
    mainCam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key) { 

        case 'f':
            ofToggleFullscreen();
            break;
        case 'z':
            perlin->addNunique(15);
            break;
        case 's':
            perlin->subNunique(15);
            break;       

        case 'u' : 
            perlin->increaseAmplitude(0.5);
            break;
        case 'd': 
            perlin->decreaseAmplitude(0.5);
            break;

        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

