#include "perlin3D.hpp"

// ====================== perlin3D class ======================  

// ======== private ========




void Perlin3D::generatePermutation(){
    rng.seed(seed);
    perm.resize(nUnique);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);

    perm.resize(nUnique*2);
    for (int i = 0 ; i < nUnique ; i ++ ){
        perm[nUnique + i] = perm[i];
    }

}

void Perlin3D::generateCells3D(){
    rng.seed(seed);
    cells3D.clear();
    for (int i = 0; i < nUnique* 2; i++) {
        float gx = randomFloat()*2-1;
        float gy = randomFloat()*2-1;
        float gz = randomFloat()*2-1;
        ofVec3f g(gx, gy, gz);
        g.normalize();
        cells3D.push_back({g.x, g.y, g.z});
    }
}


std::array<float , 3> Perlin3D::rotate(std::array<float, 3>& vect, float theta){
    float thetaRad = theta * (2.0f * M_PI / 360.0f);
    float c = std::cos(thetaRad);
    float s = std::sin(thetaRad);
    return {c* vect[0] - s* vect[1], s * vect[0] + c * vect[1], vect[2]};
}

std::vector<std::array<float , 3>> Perlin3D::rotateCells(std::vector< std::array<float, 3>>& baseCells, int theta){
    std::vector<std::array<float, 3>> res;
    res.reserve(baseCells.size());
    for (auto & v : baseCells ){
        res.push_back(rotate(v, theta));
    }
    return res;
}



float Perlin3D::dot(std::array<float , 3>& vect1, std::array<float , 3> & vect2){
    return vect1[0] * vect2[0] + vect1[1] * vect2[1] + vect1[2] * vect2[2];
}



float Perlin3D::noise3D(float x, float y , float z, float currentTime){

    x = x* scale;
    y = y * scale;
    z = z * scale + currentTime;
     // position without the unit space.
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y)& 255;
    int Z = (int)floor(z)& 255;


    float xf = x - floor(x);
    float yf = y - floor(y);
    float zf = z - floor(z);
   
    int A = perm[X] + Y;
    int AA = perm[A] + Z;
    int AB = perm[A + 1] + Z;
    int B = perm[X + 1] + Y;
    int BA = perm[B] + Z;
    int BB = perm[B + 1] + Z;

    
    std::array<float, 3> g000 = cells3D[perm[AA]];
    std::array<float, 3> g100 = cells3D[perm[BA]];
    std::array<float, 3> g010 = cells3D[perm[AB]];
    std::array<float, 3> g110 = cells3D[perm[BB]];

    std::array<float , 3> g001 = cells3D[perm[AA + 1]];
    std::array<float , 3> g101 = cells3D[perm[BA + 1]];
    std::array<float , 3> g011 = cells3D[perm[AB + 1]];
    std::array<float , 3> g111 = cells3D[perm[BB + 1]];


    std::array<float, 3> d000 = {xf, yf, zf};
    std::array<float, 3> d100 = {xf-1, yf, zf};
    std::array<float, 3> d010 = {xf, yf-1, zf};
    std::array<float, 3> d110 = {xf-1, yf-1, zf};

    std::array<float, 3> d001 = {xf, yf, zf -1};
    std::array<float, 3> d101 = {xf-1, yf, zf-1};
    std::array<float, 3> d011 = {xf, yf-1, zf-1};
    std::array<float, 3> d111 = {xf-1, yf-1, zf-1};



    float in000 = dot(g000, d000);
    float in100 = dot(g100, d100);
    float in010 = dot(g010, d010);
    float in110 = dot(g110, d110);



    float in001 = dot(g001, d001);
    float in101 = dot(g101, d101);
    float in011 = dot(g011, d011);
    float in111 = dot(g111, d111);

    float t1 = lerp(lerp(in000, in100,fade(xf)), lerp(in010, in110, fade(xf)), fade(yf));
    float t2 = lerp(lerp(in001, in101, fade(xf)), lerp(in011, in111, fade(xf)), fade(yf));

    return lerp(t1, t2, fade(zf));



}


float Perlin3D::fbm3D(float x, float y, float z, float currentTime){
    float total = 0.0f;
    float frequency = 1.0f;
    float amp = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; i++){
        total += noise3D(x * frequency, y * frequency, z * frequency, currentTime) * amp;
        maxValue += amp;
        frequency *= lacunarity;
        amp *= persistence;
    }

    return total / maxValue;
}


// ======== public ========


Perlin3D::Perlin3D(float initScale, int nUnique, ColorFunction color, unsigned int seed): Perlin(initScale, nUnique, seed), colorFunction(color){
    this->generateCells3D();
    this->generatePermutation();
}

ofFloatColor Perlin3D::getColorFromNoise(float noiseValue){
    float sill = 0.40;
    
    return colorFunction(noiseValue, sill);
}

void Perlin3D::updateRotation(float updatedTheta){
    theta = updatedTheta;
    cells3D = rotateCells(cells3D, theta);

}


void Perlin3D::setColorFunction(ColorFunction newColorFunction){
    colorFunction = newColorFunction;
}


void Perlin3D::createNewGeneration(unsigned int seed ){
    setSeed(seed);
    generateCells3D();
    generatePermutation();
}