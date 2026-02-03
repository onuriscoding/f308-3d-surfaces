#include "perlin3D.hpp"

// ====================== perlin3D class ======================  

// ======== private ========

void Perlin3D::initPermutation(){
    //init cells3D
    std::vector<std::array<float, 3>> cellsBase ;
    for (int i = 0; i < nUnique * 2 ; i ++){
        cellsBase.push_back({(this->randomFloat() - 0.5f ) * 2.0f , (this->randomFloat() - 0.5f) *2.0f, (this->randomFloat() - 0.5f) *2.0f});
    }


    cells3D = rotateCells(cellsBase, theta / 360 * 2 * M_PI);
    
    //init perm
    perm.resize(nUnique);
    std::iota(perm.begin(), perm.end(), 0);
    static std::mt19937 rng(std::time(nullptr));
    std::shuffle(perm.begin(), perm.end(), rng);


}


std::array<float , 3> Perlin3D::rotate(std::array<float, 3>& vect, int theta){

    float c = std::cos(theta);
    float s = std::sin(theta);
    return {c* vect[0] - s* vect[1], s * vect[0] - c * vect[1], vect[2]};
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



float Perlin3D::noise3D(float x, float y , float z){
     // position without the unit space.
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y)& 255;
    int Z = (int)floor(z)& 255;


    float xf = x - floor(x);
    float yf = y - floor(y);
    float zf = z - floor(z);
    


    int X0 = X% nUnique;
    int Y0 = Y% nUnique;
    int Z0 = Z% nUnique;

    int X1 = (X + 1) % nUnique;
    int Y1 = (Y + 1) % nUnique;
    int Z1 = (Z + 1) % nUnique;

    std::array<float, 3> g000 = cells3D[X0 + perm[Y0] + perm[Z0] ];
    std::array<float, 3> g100 = cells3D[X1 + perm[Y0] + perm[Z0]];
    std::array<float, 3> g010 = cells3D[X0 + perm[Y1] + perm[Z0]];
    std::array<float, 3> g110 = cells3D[X1 + perm[Y1] + perm[Z0]];


    std::array<float, 3> g001 = cells3D[X0 + perm[Y0] + perm[Z1]];
    std::array<float, 3> g101 = cells3D[X1 + perm[Y0]+ perm[Z1]];
    std::array<float, 3> g011 = cells3D[X0 + perm[Y1]+ perm[Z1]];
    std::array<float, 3> g111 = cells3D[X1 + perm[Y1]+ perm[Z1]];


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


// ======== public ========


Perlin3D::Perlin3D(float initScale, int nUnique): Perlin(initScale, nUnique){
    this->initPermutation();
}
void Perlin3D::updateMesh(ofMesh &mesh ){
    for (int i = 0; i < mesh.getNumVertices(); i++){
        ofVec3f vec = mesh.getVertex(i);
        float noiseValue = noise3D(vec.x * scale, vec.y * scale, vec.z * scale);
        // todo move 
        float sill = 0.45;
        float st = ofMap(noiseValue, sill, 1.0, 0.0,1.0, true);

        ofFloatColor col;
        col.setHsb(ofMap(noiseValue, 0, 1, 0.6, 0.95), 0.8,1.0,st);
        mesh.setColor(i, col);

    }
}