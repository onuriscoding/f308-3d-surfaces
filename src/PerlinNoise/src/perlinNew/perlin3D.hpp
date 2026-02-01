#ifndef PERLIN_3D_HPP
#define PERLIN_3D_HPP

#include "ofMain.h"
#include "../perlin.hpp"
#include "../permutation.hpp"

class Perlin3D : public Perlin {

    private:
        std::vector<std::array<float , 3>> cells3D;
        float grad(int hash, std::vector<float> coord) override;
        float lerp(float a, float b, float t) override;
        
        float perlin(std::vector<float> coord) override;

        
        float rotatePerlinNoise(std::vector<float> coord) override;
        float fade(float t) override;
        
        std::array<float, 3> rotate(std::array<float, 3>& vect, int theta); 
        float dot(std::array<float , 3>& vect1, std::array<float , 3> & vect2);
         
        float randomFloat() override;

        void initPermuation()override;



    public: 

        Perlin3D(float initScale, int nUnique, int theta);
//        ~Perlin2D() = default;

        void updateMesh(ofMesh &mesh, std::vector<int> values) override;

        void increaseAmplitude(float value) override;
        void decreaseAmplitude(float value) override;

        std::vector<std::array<float , 3>> rotateCells(std::vector< std::array<float, 3>>& baseCells , int theta);

        void addNunique(int n)override ;
        void subNunique(int n)override ;
        

        // =============== setters ===============

        void setTheta(int newTheta ) override ;
        void setScale(float newScale)override ;
        void setNunique(int newNunique)override ;
        



        // =============== getters ===============

};



#endif
