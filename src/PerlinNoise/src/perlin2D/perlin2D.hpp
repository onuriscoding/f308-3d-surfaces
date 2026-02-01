#ifndef PERLIN_2D_HPP
#define PERLIN_2D_HPP

#include "ofMain.h"
#include "random"
#include "cmath"
#include "vector"
#include "../perlin/perlin.hpp"


/**
class that handles the Perlin Noise algorithm for a 2D representation
**/
class Perlin2D : public Perlin{


    private:

        // scale of the representaiton
        //float scale; 
        //noise that repeats every nUnique units
        //int nUnique;

        // use as rotation value 
        int theta;
        // amplitude of the representaiton
        float amplitude;

        // contains the gradiant vectors  
        std::vector<std::array<float , 2>> cells2D;

        //a randomised array of the number of unique noise values we have
        std::vector<int > perm;


        /*
        brief : return a weighted average 
        */
        //float lerp(float a, float b, float t);
        

        /*
        brief : compute a noise value from a coordinate
        param x ,y : coordinate
        */
        float noise2D(float x , float y);

        /*
        brief : This allows the value of the coordinates to be smoothed so that they gradually approach integer values.
        param t :  coordinate axis (x or y) 
        */
        //float fade(float t);
        
        /*
        brief : rotates a specifics gradiant vecotor
        param vect : gradiant vector  
        param theta :  
        */
        std::array<float, 2> rotate(std::array<float, 2>& vect, int theta); 

        
        
        /*
        brief : dot product between two vectors 
        */
        float dot(std::array<float , 2>& vect1, std::array<float , 2> & vect2);
        
       


        void initPermuation();



    public: 
        // -- constructor --  
        Perlin2D(float initScale, int nUnique, int theta);
        ~Perlin2D() = default;

        
        /*
        brief : update the ofMesh object by applying Perlin noise to each coordinate
        param height, width : size of the ofMesh object 
        */
        void updateMesh(ofMesh &mesh, int height , int width);



        void increaseAmplitude(float value);
        void decreaseAmplitude(float value);
        /*
        brief : rotates all the gradiant vectors 
        */
        std::vector<std::array<float , 2>> rotateCells(std::vector< std::array<float, 2>>& baseCells , int theta);

        

        // =============== setters ===============

        void setTheta(int newTheta );
        void setScale(float newScale);
        void setNunique(int newNunique);
        



        // =============== getters ===============

};



#endif