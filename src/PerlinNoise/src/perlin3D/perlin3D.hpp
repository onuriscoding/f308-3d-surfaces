#ifndef PERLIN_3D_HPP
#define PERLIN_3D_HPP

#include "../perlin/perlin.hpp"



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

        void initPermutation();

        std::array<float , 3> rotate(std::array<float, 3>& vect, int theta);

        std::vector<std::array<float, 3>> rotateCells(std::vector< std::array<float, 3>>& baseCells, int theta);

    public : 

        Perlin3D(float initScale, int nUnique); 
        ~Perlin3D() = default;


        void updateMesh(ofMesh &mesh );
        float noise3D(float x, float y, float z);

        

        /*
        brief : dot product between two vectors 
        */
        float dot(std::array<float , 3>& vect1, std::array<float , 3> & vect2);



};



#endif