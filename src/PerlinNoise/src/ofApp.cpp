#include "ofApp.hpp"
#include <filesystem>
#include <iostream>


// ===== public ===== 
void ofApp::setup(){
    height2D = 500;
    width2D = 400;
    size3D = 80;
    spacing = 3.0f;
    mapsDir = findMapsDir();
    perlin2D = std::make_unique<Perlin2D>(0.02,250, 13.0, 4);

    perlinManager = std::make_unique<PerlinManager>(size3D);

    // camera rotation

    mainCam.setRelativeYAxis(false);
    mainCam.setUpAxis(glm::vec3(0, 0, 1));

    // --- gui setup ---

    movement.addListener(this, &ofApp::movementChangedCallBack);

    ofxGuiSetFont("mono.ttf", 13);
    ofxGuiSetDefaultWidth(410);
    ofxGuiSetDefaultHeight(30);
    ofxGuiSetTextPadding(4);

    gui.setup();
    gui.add(infoLabel.setup("controls", 
        " u/d : Amplitude  s : Save  e : Export PNG\n 1-9 : Load map 1-9  0 : map 10\n v : Split view  f : Fullscreen\n"));
    gui.add(theta.setup("rotation", 140, 0, 360));
    gui.add(renderPerlin3D.setup("render 3D Perlin noise", false));
    gui.add(movement.setup("movement", false));
    gui.add(newGeneration.setup("new noise generation "));
    gui.add(perlinScale.setup("perlin scale ",0.02, 0.001 ,0.13 ));
    gui.add(amplitudeSlider.setup("amplitude", 35.0, 1.0, 100.0));
    gui.add(octavesSlider.setup("octaves", 1, 1, 8));
    gui.add(sillSlider.setup("density threshold", 0.40, 0.05, 0.80));
    gui.add(scale3DSlider.setup("3D scale", 0.02, 0.005, 0.15));
    gui.add(seedInput.setup("seed", 4, 0, 9999));

    newGeneration.addListener(this, &ofApp::newGenerationCallback);

    gui.add(saveMapBtn.setup("save map"));
    gui.add(exportPngBtn.setup("export PNG"));
    gui.add(splitView.setup("split view", false));
    saveMapBtn.addListener(this, &ofApp::saveMapCallback);
    exportPngBtn.addListener(this, &ofApp::exportPNGCallback);


    gravel.addListener(this, &ofApp::setValPerlin3DcallBack);
    earth.addListener(this, &ofApp::setValPerlin3DcallBack);
    rock.addListener(this, &ofApp::setValPerlin3DcallBack);
    gui.add(earth.setup("earth : ", true));
    gui.add(gravel.setup("gravel : ", false));
    gui.add(rock.setup("rock : ", false));

    gui.add(currentMapLabel.setup("Current Map", " 1"));

    colorMode.addListener(this, &ofApp::colorModeCallback);
    placeTrees.addListener(this, &ofApp::placeTreesCallback);
    clearTreesButton.addListener(this, &ofApp::removeTreesCallback);

    gui.add(colorMode.setup("Color Mode", false));
    gui.add(placeTrees.setup("Place Tress"));
    gui.add(clearTreesButton.setup("Remove Trees"));


    mesh2D.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofEnableDepthTest();
    
    // -- init meshs -- 
    initMesh2D();
    initMesh3D();

    seedInput.addListener(this, &ofApp::onSeedChanged);

    
    ofBackground(0);
    ofSetColor(255);
   
    mainCam.setPosition(0, -400, 400);
    mainCam.lookAt(glm::vec3(0,0,0));

    splitCam2D.setPosition(0, 0, 600);
    splitCam2D.lookAt(glm::vec3(0,0,0));

    perlinManager->updateMesh(mesh3D);

    mapsDir = findMapsDir();
    scanMaps();

    treeModel.load(treeFile);

    light.setup();
    light.setPosition(300, -300, 600);

    generateTrees();
}

void ofApp::generateTrees() {
    int maxAttempts = 5000;
    trees.clear();
    int attempts = 0;

    while (trees.size() < targetTreeNumber && attempts < maxAttempts) {
        attempts++;

        float xPos = ofRandom(0, width2D);
        float yPos = ofRandom(0, height2D);

        float xCentered =  xPos - width2D/2;
        float yCentered = yPos - height2D/2;


        float noiseValue = perlin2D->fbm2D(xPos * perlinScale, yPos * perlinScale);

        if (noiseValue > perlin2D->shallowWaterLevel && noiseValue <= perlin2D->sandLevel) {
            trees.push_back({{xCentered,yCentered,0},{xPos,yPos,0}, ofRandom(0.8f, 1.2f)});
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    //update scale 
    if(ofGetMousePressed() && perlinScale.getShape().inside(ofGetMouseX(), ofGetMouseY())){
        dragingScaleSlider = true;
    }

    if(dragingScaleSlider && !ofGetMousePressed()){
        dragingScaleSlider = false;
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
    if (splitView){
        perlin2D->updateMesh(mesh2D, height2D, width2D);
    } else if (renderPerlin3D){
        // 3D mesh updated on parameter change only (expensive)
    }
    else {
        perlin2D->updateMesh(mesh2D, height2D, width2D );
    }
}

//--------------------------------------------------------------

void ofApp::loadMap(int mapIndex) {
    if (mapFiles.empty()) {
        std::cerr << "No maps found" << std::endl;
        return;
    }

    if (mapIndex < 0 || mapIndex >= mapFiles.size()) {
        std::cerr << "Invalid map index" << std::endl;
        return;
    }
    
    std::string mapFile = mapFiles[mapIndex];
    std::unordered_map<std::string, float> params = loadParameters(mapFile);

    if (!params.empty()) {
        setParameters(params);
        std::cout << "Loaded Map " << mapIndex + 1 << " from " << mapFile << std::endl;
        
        // Update both meshes so split view stays in sync
        perlin2D->updateMesh(mesh2D, height2D, width2D);
        perlinManager->updateMesh(mesh3D);
    } else {
        std::cerr << "Failed to load Map " << mapIndex << " from " << mapFile << std::endl;
    }
    currentMap = mapIndex;
    updateMapLabel();
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
    if (splitView){
        drawSplitView();
    } else if (renderPerlin3D){
        drawPerlin3D();
    }else {
        drawPerlin2D();
    }
    
    gui.draw();
}

void ofApp::setParameters(const std::unordered_map<std::string, float>& params) {
    if (params.find("scale") != params.end()) {
        perlin2D->setScale(params.at("scale"));
        perlinScale = params.at("scale");
    }
    if (params.find("amplitude") != params.end()) {
        perlin2D->setAmplitude(params.at("amplitude"));
        amplitudeSlider = params.at("amplitude");
    }
    if (params.find("octaves") != params.end()) {
        int oct = static_cast<int>(params.at("octaves"));
        perlin2D->setOctaves(oct);
        perlinManager->setOctaves(oct);
        octavesSlider = oct;
    }
    if (params.find("theta") != params.end()) {
        perlin2D->updateRotation(params.at("theta"));
        theta = params.at("theta");
    }
    if (params.find("seed") != params.end()) {
        unsigned int newSeed = static_cast<unsigned int>(params.at("seed"));
        perlin2D->createNewGeneration(newSeed);
        perlinManager->createNewGeneration(newSeed);
        seedInput = static_cast<int>(newSeed);
    }

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

void ofApp::drawPerlin2D() {
    ofEnableDepthTest();
    mainCam.begin();
    mesh2D.draw();
    if (treesActive) drawTrees();
    mainCam.end();
    ofDisableDepthTest();
}

void ofApp::drawTrees() {
    ofEnableLighting();
    light.enable();

    for (Object3D &tree : trees) {
        int x = ofClamp(static_cast<int>(tree.pos[0]), 0, width2D - 1);
        int y = ofClamp(static_cast<int>(tree.pos[1]), 0, height2D - 1);
        int index = x + y * width2D;

        float z = mesh2D.getVertex(index).z;

        ofPushMatrix();
        ofTranslate(tree.posCentered[0], tree.posCentered[1], z);
        ofRotateXDeg(-90);
        ofScale(tree.scale*0.01f);
        treeModel.drawFaces();
        ofPopMatrix();
    }

    light.disable();
    ofDisableLighting();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == OF_KEY_RETURN || key == OF_KEY_ESC) editingSeed = false;

    // if (!editingSeed) {
    //     if (key >= '1' && key <= '9') { loadMap(key - '0'); return; }
    //     if (key == '0') { loadMap(10); return; }
    // }

    if (!editingSeed) {
        if (key == OF_KEY_RIGHT) {
            currentMap++;
            if (currentMap >= mapFiles.size()) currentMap = 0;
            loadMap(currentMap);
            return;
        }
        if (key == OF_KEY_LEFT) {
            currentMap--;
            if (currentMap < 0) currentMap = mapFiles.size() -1;
            loadMap(currentMap);
            return;
        }
    }
    switch(key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'u':
            perlin2D->increaseAmplitude(0.5);
            amplitudeSlider = perlin2D->getAmplitude();
            break;
        case 'd':
            perlin2D->decreaseAmplitude(0.5);
            amplitudeSlider = perlin2D->getAmplitude();
            break;
        case 's':
            saveMap();
            break;
        case 'e':
            exportPNG();
            break;
        case 'v':
            splitView = !splitView;
            break;
        default: ;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    editingSeed = seedInput.getShape().inside(x, y);
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

void ofApp::newGenerationCallback(){
    unsigned int newSeed = static_cast<unsigned int>(seedInput);
    perlin2D->createNewGeneration(newSeed);
    perlinManager->createNewGeneration(newSeed);
    if (renderPerlin3D){
        perlinManager->updateMesh(mesh3D);
    } else {
        perlin2D->updateMesh(mesh2D, height2D, width2D);
    }
    std::cout << "New generation with seed: " << newSeed << std::endl;
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


// ====== split view ======

void ofApp::drawSplitView(){
    int w = ofGetWidth();
    int h = ofGetHeight();

    // Left half: 2D
    ofViewport(0, 0, w / 2, h);
    ofEnableDepthTest();
    splitCam2D.begin(ofRectangle(0, 0, w / 2, h));
    mesh2D.draw();
    splitCam2D.end();
    ofDisableDepthTest();

    // Right half: 3D
    ofViewport(w / 2, 0, w / 2, h);
    ofEnableDepthTest();
    glPointSize(5);
    glDepthMask(GL_FALSE);
    mainCam.begin(ofRectangle(w / 2, 0, w / 2, h));
    mesh3D.draw();
    glDepthMask(GL_TRUE);
    mainCam.end();
    ofDisableDepthTest();

    // Reset viewport for GUI
    ofViewport(0, 0, w, h);
}


// ====== save / export ======

#include <filesystem>

std::string ofApp::findMapsDir() {
    namespace fs = std::filesystem;

    fs::path currentDir = ofFilePath::getCurrentExeDir();

    for (int i = 0; i <= 3; ++i) {
        fs::path candidate = currentDir / "maps";

        if (fs::exists(candidate) && fs::is_directory(candidate)) {
            return candidate.string();
        }

        currentDir = currentDir.parent_path();
    }

    std::cerr << "Directory not found" << std::endl;
    return "";
}

void ofApp::saveMap(){
    int mapNum = 1;
    std::string path;
    do {
        path = mapsDir + "/map" + ofToString(mapNum) + ".txt";
        mapNum++;
    } while (ofFile(path).exists());

    std::ofstream file(path);
    if (file.is_open()){
        file << "scale=" << static_cast<float>(perlinScale) << "\n";
        file << "amplitude=" << static_cast<float>(amplitudeSlider) << "\n";
        file << "octaves=" << static_cast<int>(octavesSlider) << "\n";
        file << "theta=" << static_cast<float>(theta) << "\n";
        file << "seed=" << static_cast<int>(seedInput) << "\n";
        file.close();
        std::cout << "Map saved to: " << path << std::endl;
    } else {
        std::cerr << "Error: Could not save map to " << path << std::endl;
    }
}

void ofApp::exportPNG(){
    ofImage img;
    img.allocate(width2D, height2D, OF_IMAGE_GRAYSCALE);

    float scale = perlin2D->getScale();
    for (int y = 0; y < height2D; y++){
        for (int x = 0; x < width2D; x++){
            // Sample raw noise directly (fbm2D returns ~0..1)
            float noiseVal = perlin2D->fbm2D(x * scale, y * scale);
            unsigned char grayByte = static_cast<unsigned char>(ofClamp(noiseVal * 255, 0, 255));
            img.setColor(x, y, ofColor(grayByte));
        }
    }

    // Create exports directory next to maps
    std::string exportDir = mapsDir + "/../exports";
    ofDirectory::createDirectory(exportDir, false, true);
    std::string filename = exportDir + "/perlin_" + ofGetTimestampString() + ".png";
    img.save(filename);
    std::cout << "PNG exported to: " << filename << std::endl;
}

void ofApp::saveMapCallback(){
    saveMap();
}

void ofApp::exportPNGCallback(){
    exportPNG();
}

void ofApp::updateMapLabel() {
        currentMapLabel = ofToString(currentMap +1);
}

void ofApp::onSeedChanged(int & newSeed) {
    perlin2D->createNewGeneration(newSeed);
    perlinManager->createNewGeneration(newSeed);

    perlin2D->updateMesh(mesh2D, height2D, width2D);
    perlinManager->updateMesh(mesh3D);
}

void ofApp::scanMaps() {
    namespace fs = std::filesystem;

    mapFiles.clear();

    for (const auto& entry : fs::directory_iterator(mapsDir)) {
        if (entry.path().extension() == ".txt") {
            mapFiles.push_back(entry.path().string());
        }
    }

    std::sort(mapFiles.begin(), mapFiles.end(),
    [](const std::string& a, const std::string& b) {
        return std::stoi(std::filesystem::path(a).stem().string().substr(3)) <
               std::stoi(std::filesystem::path(b).stem().string().substr(3));
    });

    std::cout << "Found " << mapFiles.size() << " maps" << std::endl;
}

void ofApp::colorModeCallback(bool & value){
    perlin2D->colorMode = value;
}

void ofApp::placeTreesCallback(){
    generateTrees();
    treesActive = true;
}

void ofApp::removeTreesCallback(){
    trees.clear();
    treesActive = false;
}
