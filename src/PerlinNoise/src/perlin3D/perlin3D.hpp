#ifndef PERLIN_3D_HPP
#define PERLIN_3D_HPP

#include "../perlin2D/perlin2D.hpp"
#include "../colorManager3D/colorManager3D.hpp"



/**
class that handles the Perlin Noise algorithm for a 2D representation
**/

class Perlin3D : public Perlin {
    private : 
        // pas sur pour 3D 
        int theta ; 

        // contains the gradiant vectors  
        std::vector<std::array<float , 3>> cells3D;

        //a randomised array of the number of unique noise values we have
        std::vector<int > perm;

        using ColorFunction = ofFloatColor(*)(float, float);
        ColorFunction colorFunction;

        std::array<float , 3> rotate(std::array<float, 3>& vect, float theta);

        std::vector<std::array<float, 3>> rotateCells(std::vector< std::array<float, 3>>& baseCells, int theta);


        void generatePermutation();

        void generateCells3D();
        




    public : 

        Perlin3D(float initScale, int nUnique, ColorFunction color, unsigned int seed); 
        ~Perlin3D() = default;


        //void updateMesh(ofMesh &mesh );
        float noise3D(float x, float y, float z, float currentTime);

        /*
        brief : fractional Brownian Motion - stacks multiple octaves of noise3D
        for richer, multi-scale detail
        */
        float fbm3D(float x, float y, float z, float currentTime);

        ofFloatColor getColorFromNoise(float noiseValue);        


        /*
        brief : dot product between two vectors 
        */
        float dot(std::array<float , 3>& vect1, std::array<float , 3> & vect2);

        void updateRotation(float updatedTheta);

        void setColorFunction(ColorFunction newColorFunction);

        void createNewGeneration(unsigned int seed );



};



#endif