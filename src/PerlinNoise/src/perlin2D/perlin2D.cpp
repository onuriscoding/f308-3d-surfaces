#include "perlin2D.hpp"


// ================ 2D perlin Noise class ================


// ================ public ================



// ------- constructor ------- 
Perlin2D::Perlin2D(float initScale , int nUnique, int theta) : scale(initScale) , nUnique(nUnique), theta(theta), amplitude(10.0) {
    initPermuation();
}


void Perlin2D::updateMesh(ofMesh &mesh, int height , int width){
      for (int y = 0 ; y < height; y ++){
        for (int x = 0 ; x < width; x ++){
            int index = x + y * width;
            ofVec3f v = mesh.getVertex(index);
            // float nx = ((float)x + randomFloat()) * scale; 
            // float ny = ((float) y + randomFloat() ) * scale;
            float nx = x * scale; 
            float ny = y * scale;
            
            float noiseValue = perlin2D(nx, ny); 
            
            noiseValue = ofMap(noiseValue, -1, 1, 0, 1);   
            //ofFloatColor c = ofFloatColor(noiseValue, 0.3, 1.0 - noiseValue);
            v.z = noiseValue * amplitude;
            //mesh.setVertex(index, v);

            ofFloatColor c = ofFloatColor(noiseValue, 0.3, 1.0 - noiseValue);
            mesh.setColor(index, c );

        }
    }
}


std::vector<std::array<float , 2>> Perlin2D::rotateCells(std::vector< std::array<float, 2>>& baseCells, int theta){
    std::vector<std::array<float, 2>> res;
    res.reserve(baseCells.size());
    for (auto & v : baseCells ){
        res.push_back(rotate(v, theta));
    }
    return res;
}


void Perlin2D::increaseAmplitude(float value){
    amplitude += value;
}
void Perlin2D::decreaseAmplitude(float value){
    amplitude -= value;
}


//------------------ setters ------------------


void Perlin2D::setTheta(int newTheta ){
    theta = newTheta;
}
void Perlin2D::setScale(float newScale){
    scale = newScale;
}
void Perlin2D::setNunique(int newNunique){
    nUnique = newNunique;
}

//------------------ getters ------------------


// ================ private ================

void Perlin2D::initPermuation(){        

    for(int i=0; i<256; i++){
        p[i] = permutation[i];
        p[256+i] = permutation[i];
    }
    // --- init cells2D --- 
    std::vector<std::array<float, 2>> cellsBase ;
    for (int i = 0; i < nUnique * 2 ; i ++){
        cellsBase.push_back({(this->randomFloat() - 0.5f ) * 2.0f , (this->randomFloat() - 0.5f) *2.0f });
    }
    cells2D = rotateCells(cellsBase, theta / 360 * 2 * M_PI);

    // --- init perm ---
    perm.resize(nUnique);
    std::iota(perm.begin(), perm.end(), 0);
    static std::mt19937 rng(std::time(nullptr));
    std::shuffle(perm.begin(), perm.end(), rng);


}


std::array<float , 2> Perlin2D::rotate(std::array<float, 2>& vect, int theta){

    float c = std::cos(theta);
    float s = std::sin(theta);
    return {c* vect[0] - s* vect[1], s * vect[0] - c * vect[1]};
}
  





float Perlin2D::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
    //return t;
}


float Perlin2D::grad(int hash , float x,float y){
    int h = hash%3;
    if (h == 0) return x + y;
    if (h == 1) return -x + y ;
    if (h == 2) return x - y;
    return -x - y;
}

float Perlin2D::rotatePerlinNoise(float x , float y){
    
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y)& 255;
    float xf = x - floor(x);
    float yf = y - floor(y);

    int X0 = X% nUnique;
    int Y0 = Y% nUnique;
    int X1 = (X + 1) % nUnique;
    int Y1 = (Y + 1) % nUnique;
    
    std::array<float, 2> g00 = cells2D[X + perm[Y0]];
    std::array<float, 2> g10 = cells2D[X1 + perm[Y0]];
    std::array<float, 2> g01 = cells2D[X0 + perm[Y1]];
    std::array<float, 2> g11 = cells2D[X1 + perm[Y1]];
    
    std::array<float , 2> d00 = {x, y};
    std::array<float, 2> d10 = {x-1, y};
    std::array<float , 2> d01 = {x, y-1};
    std::array<float, 2> d11 = {x-1, y-1};

    
    float in00 = dot(g00, d00);
    float in10 = dot(g10, d10);
    float in01 = dot(g01, d01);
    float in11 = dot(g11, d11);
    
    float l1 = lerp(in00, in10,fade(x));
    float l2 = lerp(in01, in11, fade(x));
    
    return lerp(l1, l2, fade(y)) + 0.5;
}


float Perlin2D::perlin2D(float x, float y) {
    
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y)& 255;
    float xf = x - floor(x);
    float yf = y - floor(y);
    
    float u = fade(xf);
    float v = fade(yf);
    
    int aa = p[p[X] + Y];
    int ab = p[p[X] + Y + 1];
    int ba = p[p[X + 1] + Y];
    int bb = p[p[X + 1] + Y + 1];

    float x1 = lerp(grad(aa, xf,yf), grad(ba, xf-1.0, yf), u);
    float x2 = lerp(grad(ab, xf,yf-1.0), grad(bb, xf-1.0, yf-1.0), u);
    
    return lerp(x1, x2 , v);
}

float Perlin2D::lerp(float a, float b, float t) {
    
    return ((1-t) * a) + (t * b);
}

float Perlin2D::dot(std::array<float , 2>& vect1, std::array<float , 2> & vect2){
    return vect1[0] * vect2[0] + vect1[1] * vect2[1];
}


float Perlin2D::randomFloat(){
 
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // float ran =  (float)(rand()) / (float)(rand());
    // return abs((int)floor(ran) - ran );
}