#include "ofApp.hpp"



// ===== public ===== 
void ofApp::setup(){
    height2D = 500;
    width2D = 500;
    size3D = 80;
    spacing = 3.0f;
    perlin2D = std::make_unique<Perlin2D>(0.02,250, 13.0, 4);
    
    perlinManager = std::make_unique<PerlinManager>(size3D);
    
    // --- gui setup ---

    movement.addListener(this, &ofApp::movementChangedCallBack);
    uniquePerlin.addListener(this, &ofApp::setValPerlin3DcallBack);
    gui.setup();
    gui.setSize(300, 500);
    gui.add(infoLabel.setup("controls", 
        " u/d : increase/decrease Amplitude \n 1-9 : Load map 1-9 \n 0 : Load map 10 \n"));
    gui.add(theta.setup("rotation", 140, 0, 360));
    gui.add(renderPerlin3D.setup("render 3D Perlin noise", true));
    gui.add(movement.setup("movement", false));
    gui.add(newGeneration.setup("new noise generation "));
    gui.add(perlinScale.setup("perlin scale ",0.02, 0.001 ,0.13 ));
    gui.add(uniquePerlin.setup("unique perlin3D ", true));
    gui.add(amplitudeSlider.setup("amplitude", 35.0, 1.0, 100.0));
    gui.add(octavesSlider.setup("octaves", 1, 1, 8));
    gui.add(sillSlider.setup("density threshold", 0.40, 0.05, 0.80));
    gui.add(scale3DSlider.setup("3D scale", 0.02, 0.005, 0.15));


    gravel.addListener(this, &ofApp::setValPerlin3DcallBack);
    earth.addListener(this, &ofApp::setValPerlin3DcallBack);
    rock.addListener(this, &ofApp::setValPerlin3DcallBack);
    gui.add(earth.setup("earth : ", true));
    gui.add(gravel.setup("gravel : ", false));
    gui.add(rock.setup("rock : ", false));

    mesh2D.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofEnableDepthTest();
    
    // -- init meshs -- 
    initMesh2D();
    initMesh3D();
    
    ofBackground(0);
    ofSetColor(255);
   
    mainCam.setPosition(0, 0, 600);
    mainCam.lookAt(glm::vec3(0,0,0));

    perlinManager->updateMesh(mesh3D);
}

//--------------------------------------------------------------
void ofApp::update(){

    //update scale 
    if(ofGetMousePressed() && perlinScale.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        dragingScaleSlider = true;
    }

    if(dragingScaleSlider && !ofGetMousePressed()){
        dragingScaleSlider = false;
        //perlin3D->setScale(perlinScale);
        perlin2D->setScale(perlinScale);
    }

    // update amplitude
    if(ofGetMousePressed() && amplitudeSlider.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        draggingAmplitudeSlider = true;
    }

    if(draggingAmplitudeSlider && !ofGetMousePressed()){
        draggingAmplitudeSlider = false;
        perlin2D->setAmplitude(amplitudeSlider);
    }

    // update octaves
    if(ofGetMousePressed() && octavesSlider.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        draggingOctavesSlider = true;
    }

    if(draggingOctavesSlider && !ofGetMousePressed()){
        draggingOctavesSlider = false;
        perlin2D->setOctaves(octavesSlider);
        perlinManager->setOctaves(octavesSlider);
        perlinManager->updateMesh(mesh3D);
    }

    // update sill (density threshold)
    if(ofGetMousePressed() && sillSlider.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        draggingSillSlider = true;
    }

    if(draggingSillSlider && !ofGetMousePressed()){
        draggingSillSlider = false;
        perlinManager->setSill(sillSlider);
        perlinManager->updateMesh(mesh3D);
    }

    // update 3D scale
    if(ofGetMousePressed() && scale3DSlider.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        draggingScale3DSlider = true;
    }

    if(draggingScale3DSlider && !ofGetMousePressed()){
        draggingScale3DSlider = false;
        perlinManager->setScale3D(scale3DSlider);
        perlinManager->updateMesh(mesh3D);
    }

    // update rotation
    if(ofGetMousePressed() && theta.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        draggingThetaSlider = true;
    }

    if(draggingThetaSlider && !ofGetMousePressed()){
        draggingThetaSlider = false;
        perlin2D->updateRotation(theta);
    }

    //update mesh
    if (renderPerlin3D){
        //perlinManager->updateMesh(mesh3D);   
    }
    else {
        perlin2D->updateMesh(mesh2D, height2D, width2D );
    }
}

//--------------------------------------------------------------

// ! [LAUNCH FROM PerlinNoise/bin] !
void ofApp::loadMap(int mapNumber) {
    if (mapNumber < 1 || mapNumber > 10) {
        std::cerr << "Invalid map number: " << mapNumber << ". Must be between 1 and 10." << std::endl;
        return;
    }
    
    // ! [LAUNCH FROM PerlinNoise/bin] !
    std::string mapFile = "../maps/map" + std::to_string(mapNumber) + ".txt";
    std::unordered_map<std::string, float> params = loadParameters(mapFile);

    if (!params.empty()) {
        setParameters(params);
        std::cout << "Loaded Map " << mapNumber << " from " << mapFile << std::endl;
        
        // Update mesh after render
        if (renderPerlin3D) {
            perlinManager->updateMesh(mesh3D);
        } else {
            perlin2D->updateMesh(mesh2D, height2D, width2D);
        }
    } else {
        std::cerr << "Failed to load Map " << mapNumber << " from " << mapFile << std::endl;
    }
}

std::unordered_map<std::string, float> ofApp::loadParameters(const std::string &filename) {
    std::unordered_map<std::string, float> params;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return params;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        float value;

        if (std::getline(iss, key, '=') && iss >> value) {
            params[key] = value;
        }
    }

    return params;
}

void ofApp::draw(){
    if (renderPerlin3D){
        drawPerlin3D();
    }else {
        drawPerlin2D();
    }
    
    gui.draw();
}

void ofApp::setParameters(const std::unordered_map<std::string, float>& params) {
    if (!perlin2D) {
        std::cerr << "Error: perlin2D is not initialized!" << std::endl;
        return;
    }

    std::cout << "Setting parameters..." << std::endl;

    if (params.find("scale") != params.end()) {
        std::cout << "Scale: " << params.at("scale") << std::endl;
        perlin2D->setScale(params.at("scale"));
        perlinScale = params.at("scale");
    }
    if (params.find("amplitude") != params.end()) {
        std::cout << "Amplitude: " << params.at("amplitude") << std::endl;
        perlin2D->setAmplitude(params.at("amplitude"));
        amplitudeSlider = params.at("amplitude");
    }
    if (params.find("octaves") != params.end()) {
        std::cout << "Octaves: " << params.at("octaves") << std::endl;
        perlin2D->setOctaves(static_cast<int>(params.at("octaves")));
        octavesSlider = static_cast<int>(params.at("octaves"));
    }
    if (params.find("theta") != params.end()) {
        std::cout << "Theta: " << params.at("theta") << std::endl;
        perlin2D->updateRotation(params.at("theta"));
        theta = params.at("theta");
    }
    if (params.find("seed") != params.end()) {
        std::cout << "Seed: " << params.at("seed") << std::endl;
        perlin2D->setSeed(static_cast<unsigned int>(params.at("seed")));
    }

    std::cout << "Parameters set successfully." << std::endl;
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
            amplitudeSlider = perlin2D->getAmplitude();
            break;
        case 'd':
            perlin2D->decreaseAmplitude(0.5);
            amplitudeSlider = perlin2D->getAmplitude();
            break;
        
        case '1':
            loadMap(1);
            break;
        case '2':
            loadMap(2);
            break;
        case '3':
            loadMap(3);
            break;
        case '4':
            loadMap(4);
            break;
        case '5':
            loadMap(5);
            break;
        case '6':
            loadMap(6);
            break;
        case '7':
            loadMap(7);
            break;
        case '8':
            loadMap(8);
            break;
        case '9':
            loadMap(9);
            break;
        case '0':
            loadMap(10);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

void ofApp::movementChangedCallBack(bool & value){
    perlin2D->setMovementVal(value);
    perlinManager->setMovementVal(value);
}

void ofApp::setValPerlin3DcallBack(bool & value){
    perlinManager->setCaveVal(gravel);
    perlinManager->setEarthVal(earth);
    perlinManager->setRockVal(rock);
    perlinManager->updateMesh(mesh3D);
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