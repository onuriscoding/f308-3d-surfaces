#ifndef PERLIN__HPP
#define PERLIN_HPP

#include "permutation.hpp"
#include "ofMain.h"
#include "random"
#include "cmath"
#include "vector"


class Perlin{


    private:
        float scale; 
        int nUnique;
        int theta;
        float amplitude;

        int p[512];
        std::vector<int> perm;

        virtual float grad(int hash , std::vector<float> coord)=0;
        virtual float lerp(float a, float b, float t)=0;        
        virtual float perlin(std::vector<float> coord)=0;
        
        virtual float rotatePerlinNoise(std::vector<float> coord)=0;
        virtual float fade(float t)=0;        
        
        virtual float randomFloat()=0;

        virtual void initPermuation()=0;

    public: 

        Perlin(float initScale, int nUnique, int theta, float amplitude);
        virtual ~Perlin();
// values : 0 - height, 1 - width, 2 - depth 
        virtual void updateMesh(ofMesh &mesh,std::vector<int> values)=0;

        virtual void increaseAmplitude(float value)=0;
        virtual void decreaseAmplitude(float value)=0;

        virtual void addNunique(int n)=0;
        virtual void subNunique(int n)=0;
        

        // =============== setters ===============

        virtual void setTheta(int newTheta )=0;
        virtual void setScale(float newScale)=0;
        virtual void setNunique(int newNunique)=0;
        



        // =============== getters ===============

};



#endif
