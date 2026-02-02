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


    return 10.2f;
}


// ======== public ========


Perlin3D::Perlin3D(float initScale, int nUnique): Perlin(initScale, nUnique){
    this->initPermutation();
}
void Perlin3D::updateMesh(ofMesh &mesh ){

}