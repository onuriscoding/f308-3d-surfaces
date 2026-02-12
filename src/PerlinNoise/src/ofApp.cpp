#include "ofApp.hpp"



// ===== public ===== 
void ofApp::setup(){
    height2D = 500;
    width2D = 500;
    size3D = 80;
    spacing = 3.0f;
    perlin2D = std::make_unique<Perlin2D>(0.02,250, 13.0);
    perlin3D = std::make_unique<Perlin3D>(0.02, 256);
    
    // --- gui setup ---



    movement.addListener(this, &ofApp::movementChangedCallBack);
    gui.setup();
    //infoLabel.setup("controls", "gfesf");
    gui.setSize(300, 500);
    gui.add(infoLabel.setup("controls", 
        " u/d : increase/decrease Amplitude \n"));
    gui.add(theta.setup("rotation", 140, 0, 360));
    gui.add(renderPerlin3D.setup("render 3D Perlin noise", true));
    gui.add(movement.setup("movement", true));
    gui.add(newGeneration.setup("new noise generation "));
    gui.add(perlinScale.setup("perlin scale ",0.02, 0.001 ,0.13 ));
    



    mesh2D.setMode(OF_PRIMITIVE_TRIANGLES);

    
    
    ofEnableDepthTest();
    

    // -- init meshs -- 
    initMesh2D();
    initMesh3D();


    
    
    
    ofBackground(0);
    ofSetColor(255);

   
    mainCam.setPosition(0, 0, 600);
    mainCam.lookAt(glm::vec3(0,0,0));


}

//--------------------------------------------------------------
void ofApp::update(){

    //update scale 
    if(ofGetMousePressed() && perlinScale.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        dragingScaleSlider = true;
    }

    if(dragingScaleSlider && !ofGetMousePressed()){
        dragingScaleSlider = false;
        perlin3D->setScale(perlinScale);
        perlin2D->setScale(perlinScale);
        
    }


    // update rotation 
    if(ofGetMousePressed() && theta.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        draggingThetaSlider = true;
    }

    if(draggingThetaSlider && !ofGetMousePressed()){
        draggingThetaSlider = false;
        perlin2D->updateRotation(theta);
        perlin3D->updateRotation(theta);
        
    }




    //update mesh
    if (renderPerlin3D){
        perlin3D->updateMesh(mesh3D);

    }
    else {
        perlin2D->updateMesh(mesh2D, height2D, width2D );
    }
    

    
}




//--------------------------------------------------------------
void ofApp::draw(){
    if (renderPerlin3D){
        drawPerlin3D();
        // draw gui for perlin3D


    }else {
        drawPerlin2D();
    }
    gui.draw();
}

void ofApp::drawPerlin3D(){
    ofEnableDepthTest();
    glPointSize(5);
    glDepthMask(GL_FALSE);
    mainCam.begin();
    mesh3D.draw();
    glDepthMask(GL_TRUE); 
    mainCam.end();
    ofDisableDepthTest();
}

void ofApp::drawPerlin2D(){
    ofEnableDepthTest();
    glPointSize(5);
    mainCam.begin();
    mesh2D.draw();
    mainCam.end();
    ofDisableDepthTest();
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key) { 

        case 'f':
            ofToggleFullscreen();
            break;   
        case 'u' : 
            perlin2D->increaseAmplitude(0.5);
            break;
        case 'd': 
            perlin2D->decreaseAmplitude(0.5);
            break;

        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

void ofApp::movementChangedCallBack(bool & value){
    perlin2D->setMovementVal(value);
    perlin3D->setMovementVal(value);

}


// ====== private ======


void ofApp::initMesh2D(){
    for (int y = 0; y < height2D; y++){
        for (int x = 0; x < width2D; x++){
            mesh2D.addVertex(glm::vec3(x - width2D  * 0.5f, y - height2D * 0.5f, 0));
            mesh2D.addColor(ofFloatColor(1.0)); 
        }
    }

    
    for(int y = 0; y < height2D - 1; y++){
        for(int x = 0; x < width2D - 1; x++){
      
            mesh2D.addIndex(x+y* width2D);
            mesh2D.addIndex((x+ 1) + y*width2D);
            mesh2D.addIndex(x+ (y + 1)*width2D);
            mesh2D.addIndex((x + 1) +y*width2D);
            mesh2D.addIndex((x + 1) + (y + 1) *width2D);
            mesh2D.addIndex(x+ (y + 1) *width2D);
        }
    }
    

}


void ofApp::initMesh3D(){
    ofEnableAlphaBlending();
    glPointSize(3);
    mesh3D.setMode(OF_PRIMITIVE_POINTS);
    for (int z = 0 ; z < size3D; z ++){
        for (int y = 0 ; y < size3D; y ++){
            for (int x = 0; x < size3D; x ++){
                ofVec3f vec((x - size3D / 2 )* spacing, (y - size3D / 2 )* spacing , (z - size3D / 2 )* spacing );
                mesh3D.addVertex(vec);
                mesh3D.addColor(ofFloatColor(0, 0, 0, 0));

            } 
        }
    }

}
