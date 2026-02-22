#include "perlinManager.hpp"

// ==================== perlin manager class ====================


// ==================== private ====================


void PerlinManager::initPerlins(){
    perlins.push_back(std::make_unique<Perlin3D>(0.02, 256, ColorManager::purpleColor, 4));
    perlins.push_back(std::make_unique<Perlin3D>(0.15, 256, ColorManager::yellowColor, 12));
    noiseBlend = std::make_unique<Perlin3D>(0.02, 256, ColorManager::purpleColor, 23);
    

}


float PerlinManager::smoothstep(float edge0, float edge1, float x){
    float t = ofClamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}


// ==================== public ====================
PerlinManager::PerlinManager(int sizeMesh):  timeScale(0.2f), currentTime(0.0f), movement(true), uniquePerlin(true), size(sizeMesh){
    initPerlins();


}


void PerlinManager::updateMesh(ofMesh &mesh ){
    float depth = 0;
    float factor = 0;

    if (movement){
        currentTime = ofGetElapsedTimef() * timeScale;
    }
    
    for (int i = 0; i < mesh.getNumVertices(); i++){
        ofVec3f vec = mesh.getVertex(i);
        ofFloatColor finalColor = ofFloatColor(0, 0, 0, 0);
        float noiseValue = perlins[0]->noise3D(vec.x, vec.y, vec.z, 0);
        depth =vec.y/size ;
        factor = 1.0f - depth;

        // mask 
        float mask = noiseBlend->noise3D(vec.x, vec.y, vec.z, 0.0f);
        mask  = (mask + 1.0f) * 0.5f;
        float t = smoothstep(0.4, 0.6, mask);

      
        if (earth){
            float earthNose  = (noiseValue + 1.0f) *0.5f;
            float threshold = ofMap(factor, 0.0, 1.0, 0.3, 0.6);
            if (earthNose > threshold){
                finalColor =  ColorManager::yellowColor(earthNose, 0.40f);
            }
        }
        if (rock){
            //noiseValue = (noiseValue + 1.0f) *0.5f;
            ofFloatColor colorB = ColorManager::purpleColor(noiseValue, 0.20f);
            finalColor = finalColor.getLerped(colorB, t) ;
        } 
        
        if (gravel){
            // todo : remove threshold
            float threshold = 0.85;
        
            noiseValue = pow(noiseValue, 3.0);
            float gravelNoise  = perlins[1]->noise3D(vec.x, vec.y, vec.z, 0);
            ofFloatColor colorC = ColorManager::diamondColor(gravelNoise, 0.20f);
            finalColor = finalColor.getLerped(colorC, t) ;
        
           

        }


        mesh.setColor(i, finalColor);




        // float noiseValueA = perlins[0]->noise3D(vec.x, vec.y, vec.z, currentTime );   
        // ofFloatColor colorA = perlins[0]->getColorFromNoise(noiseValueA);

        // if (uniquePerlin){
        //     mesh.setColor(i,colorA);
        // }else {
        //     float noiseValueB = perlins[1]->noise3D(vec.x, vec.y, vec.z, currentTime);
        //     ofFloatColor colorB = perlins[1]->getColorFromNoise(noiseValueB);
        //     float mask = noiseBlend->noise3D(vec.x , vec.y ,vec.z, 0.0f);
        //     mask = (mask + 1.0f) * 0.5f;
        //     float t = smoothstep(0.4, 0.6, mask);
        //     //std::cout << "t value : " << t << " ; mask value : "  << mask<<std::endl;
        //     ofFloatColor final = colorA.getLerped(colorB, t);
        //     mesh.setColor(i,final);
        // }
    
     
       

        


    }
}


void PerlinManager::earthMesh(ofMesh &mesh){

    int H = 80;
    float depth = 0;
    float factor = 0;
    ofFloatColor finalColor;
    for (int i = 0; i < mesh.getNumVertices(); i ++){
        ofVec3f vec = mesh.getVertex(i);
        depth =vec.y/H ;
        factor = 1.0f - depth;
        float noiseValue = perlins[0]->noise3D(vec.x, vec.y, vec.z, 0);
        noiseValue = (noiseValue + 1.0f) *0.5f;
        float density = noiseValue * factor;

        float threshold = ofMap(factor, 0.0, 1.0, 0.3, 0.6);
        if (noiseValue > threshold){
            finalColor =  ColorManager::yellowColor(noiseValue, 0.40f);
            mesh.setColor(i, finalColor);
        }




        
        

    }


}



// ===== setters =====
void PerlinManager::setMovementVal(bool val){
    movement = val;
}

void PerlinManager::setRockVal(bool val){
    rock  = val;
}
void PerlinManager::setEarthVal(bool val){
    earth = val;
}
void PerlinManager::setCaveVal(bool val){
    gravel = val;
}


// ===== getters =====

