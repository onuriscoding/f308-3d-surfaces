#ifndef PERLIN__HPP
#define PERLIN_HPP

#include "ofMain.h"
#include "random"
#include "cmath"
#include "vector"


/**
parent class that contains the common elements for the application of the Perlin algorithm in all dimensions ( 3D, 2D, 1D)
**/
class Perlin {
    private : 
       
    protected: 
         // scale of the representaiton
        float scale ;
        //noise that repeats every nUnique units
        int nUnique;


        float timeScale;
        float currentTime;

        bool movement;

        
        /*
        brief : return a weighted average 
        */
        float lerp(float a, float b, float t);
        /*
        brief : This allows the value of the coordinates to be smoothed so that they gradually approach integer values.
        param t :  coordinate axis (x or y) 
        */
        float fade(float t);
        /*
        brief :generates a random float
        */
        float randomFloat();

    public : 
        //=== constructor ===
        Perlin(float initScale, int nUnique);
        ~Perlin()=default ;

        // === setters ===
        void setNunique(int newNunique);
        void setScale(float newScale);

        void setTimeScale(float newTimeScale);

        void setMovementVal(bool val);

        // === getters ===


        float getTimeScale();
        int getUnique(); 
        int getScale();


};


#endif
