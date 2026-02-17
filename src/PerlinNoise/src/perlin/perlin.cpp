

#include "perlin.hpp"


// ============ Perlin class ============ 


// ========= private =========



// ========= protected =========

float Perlin::lerp(float a, float b, float t) {
    return ((1-t) * a) + (t * b);
}



float Perlin::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
    //return t;
}

float Perlin::randomFloat(){
 
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // float ran =  (float)(rand()) / (float)(rand());
    // return abs((int)floor(ran) - ran );
}



// ========= public =========
Perlin::Perlin(float initScale , int nUnique,unsigned int seed) : scale(initScale) , nUnique(nUnique), timeScale(0.2f), currentTime(0.0f), seed(seed){
    
}


// ==== setters ====

void Perlin::setScale(float newScale){
    scale = newScale;
}
void Perlin::setNunique(int newNunique){
    nUnique = newNunique;
}

void Perlin::setTimeScale(float newTimeScale){
    timeScale = newTimeScale;
}


void Perlin::setMovementVal(bool val){
    movement = val;
}

void Perlin::setSeed(unsigned int newSeed){
    seed = newSeed;
}

// ==== getters ====

float Perlin::getTimeScale(){
    return timeScale;
}


int Perlin::getUnique(){
    return nUnique;
} 


int Perlin::getScale(){
    return scale;
}