

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
 
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng);
}



// ========= public =========
Perlin::Perlin(float initScale , int nUnique,unsigned int seed) : scale(initScale) , nUnique(nUnique), timeScale(0.2f), currentTime(0.0f), seed(seed), rng(seed), octaves(1), lacunarity(2.0f), persistence(0.5f){

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
    rng.seed(seed);
}

// ==== getters ====

float Perlin::getTimeScale(){
    return timeScale;
}


int Perlin::getUnique(){
    return nUnique;
} 


float Perlin::getScale(){
    return scale;
}

void Perlin::setOctaves(int newOctaves){
    octaves = newOctaves;
}

void Perlin::setLacunarity(float newLacunarity){
    lacunarity = newLacunarity;
}

void Perlin::setPersistence(float newPersistence){
    persistence = newPersistence;
}

int Perlin::getOctaves(){
    return octaves;
}

float Perlin::getLacunarity(){
    return lacunarity;
}

float Perlin::getPersistence(){
    return persistence;
}