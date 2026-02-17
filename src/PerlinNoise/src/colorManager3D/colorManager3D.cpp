#include "colorManager3D.hpp"


// ================== color Manager class ================== 

ofFloatColor ColorManager::purpleColor(float noiseValue, float sill ){
    float st = ofMap(noiseValue, sill, 1.0, 0.0,1.0, true);
    ofFloatColor col;
    col.setHsb(ofMap(noiseValue, 0, 1, 0.6, 0.95), 0.8,1.0,st);
    return col;
}

ofFloatColor ColorManager::goldColor(float noiseValue, float sill){
    float hue = ofMap(noiseValue, 0.20, 1.0, 0.55, 0.65); 
    ofFloatColor col;
    col.setHsb(hue, 0.9, 1.0);
    return col;
}

ofFloatColor ColorManager::rockColor(float noiseValue, float sill){
    float brightness = ofMap(noiseValue, 0.0, 1.0, 0.2, 0.5);
    return ofFloatColor(0.3, 0.25, 0.2, brightness);
}

ofFloatColor ColorManager::yellowColor(float noiseValue, float sill){
    float st = ofMap(noiseValue, sill, 1.0, 0.0, 1.0, true);
    ofFloatColor col;
    col.setHsb(ofMap(noiseValue, 0, 1, 0.12, 0.18),  0.8,1.0, st);
    return col;
}
