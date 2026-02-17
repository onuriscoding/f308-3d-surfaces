#include "perlin2D.hpp"


// ================ 2D perlin Noise class ================


// ================ public ================



// ------- constructor ------- 
Perlin2D::Perlin2D(float initScale , int nUnique, float theta, unsigned int seed) : Perlin(initScale, nUnique, seed), thetaRad(theta* (2.0f * M_PI / 360.0f)), amplitude(35.0) {
    generateCells();
    generatePerms();
}


void Perlin2D::updateMesh(ofMesh &mesh, int height , int width){
    
    if (movement){
        currentTime = ofGetElapsedTimef() * timeScale;
    }
    for (int y = 0 ; y < height; y ++){
        for (int x = 0 ; x < width; x ++){
            int index = x + y * width;
            ofVec3f v = mesh.getVertex(index);
        
            float noiseValue = noise2D(x * scale, y * scale + currentTime); 
            
            //noiseValue = ofMap(noiseValue, -1, 1, 0, 1);   
            
            v.z = noiseValue * amplitude;
            mesh.setVertex(index, v);
            ofFloatColor c = ofFloatColor(noiseValue, 0.3, 1.0 - noiseValue);
            mesh.setColor(index, c );

        }
}
}


std::vector<std::array<float , 2>> Perlin2D::rotateCells(std::vector< std::array<float, 2>>& baseCells){
    std::vector<std::array<float, 2>> res;
    res.reserve(baseCells.size());
    for (auto & v : baseCells ){
        res.push_back(rotate(v));
    }
    return res;
}


void Perlin2D::increaseAmplitude(float value){
    amplitude += value;
}
void Perlin2D::decreaseAmplitude(float value){
    amplitude -= value;
}


void Perlin2D::updateRotation(float updatedTheta){
    thetaRad = updatedTheta * (2.0f * M_PI / 360.0f);
    //thetaRad = updatedTheta;
    cells2D = rotateCells(cells2D);

}



//------------------ setters ------------------




//------------------ getters ------------------


float Perlin2D::getTheta(){
    return thetaRad;
}

// ================ private ================




void Perlin2D::generateCells(){
    srand(seed);
    std::vector<std::array<float, 2>> cellsBase ;
    for (int i = 0; i < nUnique * 2 ; i ++){
        cellsBase.push_back({(this->randomFloat() - 0.5f ) * 2.0f , (this->randomFloat() - 0.5f) *2.0f });
    }    
    cells2D = rotateCells(cellsBase);

}
void Perlin2D::generatePerms(){
    srand(seed);

    perm.resize(nUnique);
    std::iota(perm.begin(), perm.end(), 0);
    static std::mt19937 rng(std::time(nullptr));
    std::shuffle(perm.begin(), perm.end(), rng);

}


std::array<float , 2> Perlin2D::rotate(std::array<float, 2>& vect){
    
    float c = std::cos(thetaRad);
    float s = std::sin(thetaRad);
    return {c* vect[0] - s* vect[1], s * vect[0] + c * vect[1]};
}
  








float Perlin2D::noise2D(float x , float y){
    // position without the unit space.
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y)& 255;


    float xf = x - floor(x);
    float yf = y - floor(y);
    // get the left and right and bottom and top of the cell.
    int X0 = X% nUnique;
    int Y0 = Y% nUnique;
    int X1 = (X + 1) % nUnique;
    int Y1 = (Y + 1) % nUnique;
    

    // get our gradiant vectors
    std::array<float, 2> g00 = cells2D[X + perm[Y0]];
    std::array<float, 2> g10 = cells2D[X1 + perm[Y0]];
    std::array<float, 2> g01 = cells2D[X0 + perm[Y1]];
    std::array<float, 2> g11 = cells2D[X1 + perm[Y1]];
    // get the distance between a given point and the four corner of the cell
    std::array<float , 2> d00 = {xf, yf};
    std::array<float, 2> d10 = {xf-1, yf};
    std::array<float , 2> d01 = {xf, yf-1};
    std::array<float, 2> d11 = {xf-1, yf-1};

    // dot between the distance vect and the gradiant vectors 
    float in00 = dot(g00, d00);
    float in10 = dot(g10, d10);
    float in01 = dot(g01, d01);
    float in11 = dot(g11, d11);
    
    // interpolation First on the x axis for the top and bottom pairs of points
    float l1 = lerp(in00, in10,fade(xf));
    float l2 = lerp(in01, in11, fade(xf));
    // then on the y axis 
    return lerp(l1, l2, fade(yf)) + 0.5;
}



float Perlin2D::dot(std::array<float , 2>& vect1, std::array<float , 2> & vect2){
    return vect1[0] * vect2[0] + vect1[1] * vect2[1];
}


