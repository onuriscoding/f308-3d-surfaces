#include "perlinManager.hpp"

// ==================== perlin manager class ====================


// ==================== private ====================


void PerlinManager::initPerlins(){
    perlins.push_back(std::make_unique<Perlin3D>(0.02, 256, ColorManager::purpleColor, 4));
    perlins.push_back(std::make_unique<Perlin3D>(0.02, 256, ColorManager::yellowColor, 12));
    noiseBlend = std::make_unique<Perlin3D>(0.02, 256, ColorManager::purpleColor, 23);
    

}


float PerlinManager::smoothstep(float edge0, float edge1, float x){
    float t = ofClamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}


// ==================== public ====================
PerlinManager::PerlinManager():  timeScale(0.2f), currentTime(0.0f), movement(true), uniquePerlin(true){
    initPerlins();


}


void PerlinManager::updateMesh(ofMesh &mesh ){

    if (movement){
        currentTime = ofGetElapsedTimef() * timeScale;
    }
    
    for (int i = 0; i < mesh.getNumVertices(); i++){
        ofVec3f vec = mesh.getVertex(i);


        float noiseValueA = perlins[0]->noise3D(vec.x, vec.y, vec.z, currentTime );   
        ofFloatColor colorA = perlins[0]->getColorFromNoise(noiseValueA);

        if (uniquePerlin){
            mesh.setColor(i,colorA);
        }else {
            float noiseValueB = perlins[1]->noise3D(vec.x, vec.y, vec.z, currentTime);
            ofFloatColor colorB = perlins[1]->getColorFromNoise(noiseValueB);
            float mask = noiseBlend->noise3D(vec.x , vec.y ,vec.z, 0.0f);
            mask = (mask + 1.0f) * 0.5f;
            float t = smoothstep(0.4, 0.6, mask);
            //std::cout << "t value : " << t << " ; mask value : "  << mask<<std::endl;
            ofFloatColor final = colorA.getLerped(colorB, t);
            mesh.setColor(i,final);
        }
    
     
       

        


    }
}



// ===== setters =====
void PerlinManager::setMovementVal(bool val){
    movement = val;
}

void PerlinManager::setUniquePerlin(bool val){
    uniquePerlin = val;
}

// ===== getters =====

