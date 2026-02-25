#include "perlinManager.hpp"

// ==================== perlin manager class ====================


// ==================== private ====================


void PerlinManager::initPerlins(){
    perlins.push_back(std::make_unique<Perlin3D>(0.02, 256, ColorManager::purpleColor, 4));
    perlins.push_back(std::make_unique<Perlin3D>(0.02, 256, ColorManager::yellowColor, 12));
    perlins.push_back(std::make_unique<Perlin3D>(0.02, 256, ColorManager::rockColor, 37));
    perlins.push_back(std::make_unique<Perlin3D>(0.15, 256, ColorManager::yellowColor, 12));
    noiseBlend = std::make_unique<Perlin3D>(0.02, 256, ColorManager::purpleColor, 23);
    noiseBlend2 = std::make_unique<Perlin3D>(0.02, 256, ColorManager::purpleColor, 51);

}


float PerlinManager::smoothstep(float edge0, float edge1, float x){
    float t = ofClamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}


// ==================== public ====================
PerlinManager::PerlinManager(int sizeMesh):  timeScale(0.2f), currentTime(0.0f), movement(true), earth(true), rock(false), gravel(false), size(sizeMesh), sill(0.40f){
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
        float noiseValue = perlins[0]->fbm3D(vec.x, vec.y, vec.z, currentTime);
        depth = vec.y / size;
        factor = 1.0f - depth;

        // mask
        float mask = noiseBlend->noise3D(vec.x, vec.y, vec.z, 0.0f);
        mask = (mask + 1.0f) * 0.5f;
        float t = smoothstep(0.4, 0.6, mask);

        if (earth){
            float earthNoise = (noiseValue + 1.0f) * 0.5f;
            float threshold = ofMap(factor, 0.0, 1.0, 0.3, 0.6);
            // smooth alpha fade around threshold instead of hard cutoff
            float earthAlpha = smoothstep(threshold - 0.05f, threshold + 0.05f, earthNoise);
            if (earthAlpha > 0.0f){
                ofFloatColor earthColor = ColorManager::yellowColor(earthNoise, sill);
                earthColor.a *= earthAlpha;
                finalColor = earthColor;
            }
        }
        if (rock){
            float rockNoise = perlins[2]->fbm3D(vec.x, vec.y, vec.z, currentTime);
            ofFloatColor colorB = ColorManager::purpleColor(rockNoise, sill * 0.5f);
            finalColor = finalColor.getLerped(colorB, t);
        }

        if (gravel){
            float gravelNoise = perlins[1]->fbm3D(vec.x, vec.y, vec.z, currentTime);
            ofFloatColor colorC = ColorManager::diamondColor(gravelNoise, sill * 0.5f);

            // blend gravel using second mask for spatial variety
            float mask2 = noiseBlend2->noise3D(vec.x, vec.y, vec.z, 0.0f);
            mask2 = (mask2 + 1.0f) * 0.5f;
            float t2 = smoothstep(0.3, 0.7, mask2);
            finalColor = finalColor.getLerped(colorC, t2);
        }

        mesh.setColor(i, finalColor);
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

void PerlinManager::setOctaves(int val){
    for (auto& p : perlins){
        p->setOctaves(val);
    }
}

void PerlinManager::setSill(float val){
    sill = val;
}

void PerlinManager::setScale3D(float val){
    for (auto& p : perlins){
        p->setScale(val);
    }
}

void PerlinManager::createNewGeneration(unsigned int seed){
    for (auto& p : perlins){
        p->createNewGeneration(seed);
        seed += 7; // offset each perlin so they don't all share the same pattern
    }
    noiseBlend->createNewGeneration(seed + 100);
    noiseBlend2->createNewGeneration(seed + 200);
}

// ===== getters =====

