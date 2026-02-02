#include "ofApp.hpp"




void ofApp::setup(){
    perlin = std::make_unique<Perlin2D>(0.02,250, 13.0);
    

    gui.setup();
    //infoLabel.setup("controls", "gfesf");
    gui.setSize(300, 500);
    gui.add(infoLabel.setup("controls", 
        " u/d : increase/decrease Amplitude \n"));
    gui.add(theta.setup("rotation", 140, 0, 360));
    mainMesh.setMode(OF_PRIMITIVE_TRIANGLES);

    
    
    ofEnableDepthTest();
    
    
    width = 500;
    height = 500;
    ofBackground(0);
    ofSetColor(255);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            mainMesh.addVertex(glm::vec3(x - width  * 0.5f, y - height * 0.5f, 0));
            mainMesh.addColor(ofFloatColor(1.0)); 
        }
    }

    
    for(int y = 0; y < height - 1; y++){
        for(int x = 0; x < width - 1; x++){
      
            mainMesh.addIndex(x+y* width);
            mainMesh.addIndex((x+ 1) + y*width);
            mainMesh.addIndex(x+ (y + 1)*width);
            mainMesh.addIndex((x + 1) +y*width);
            mainMesh.addIndex((x + 1) + (y + 1) *width);
            mainMesh.addIndex(x+ (y + 1) *width);
        }
    }
    mainCam.setPosition(0, 0, 600);
    mainCam.lookAt(glm::vec3(0,0,0));


}

//--------------------------------------------------------------
void ofApp::update(){
    if(ofGetMousePressed() && theta.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        draggingSlider = true;
    }

    if(draggingSlider && !ofGetMousePressed()){
        draggingSlider = false;
        perlin->updateRotation(theta);
    }
    // std::cout << "slider theta : " << theta << " / perlin theta : " << perlin->getTheta() << std::endl; 
    // if(theta != perlin->getTheta()){
        
    //     perlin->updateRotation(theta);
    // }

    perlin->updateMesh(mainMesh, height, width );

   
    
    
    
}




//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    glPointSize(5);
    mainCam.begin();
    mainMesh.draw();
    mainCam.end();
    ofDisableDepthTest();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key) { 

        case 'f':
            ofToggleFullscreen();
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

