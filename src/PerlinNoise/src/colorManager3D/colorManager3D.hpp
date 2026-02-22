#ifndef COLOR_MANAGER3D_HPP
#define COLOR_MANAGER3D_HPP



#include "ofMain.h"
#include <ofShader.h>

class ColorManager  {
    public : 
        static ofFloatColor purpleColor(float noiseValue, float sill );
        static ofFloatColor yellowColor(float noiseValue, float sill);
        static ofFloatColor rockColor(float noiseValue, float sill);
        static ofFloatColor goldColor(float noiseValue, float sill);
        static ofFloatColor diamondColor(float noiseValue, float sill);
        static ofFloatColor gravelColor(float noiseValue, float sill);



        

};




#endif

