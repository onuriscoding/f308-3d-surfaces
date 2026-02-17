#ifndef PERLIN_MANAGER_HPP
#define PERLIN_MANAGER_HPP


#include "../perlin3D/perlin3D.hpp"

class PerlinManager {
    private:

        std::vector<std::unique_ptr<Perlin3D>> perlins;
        
        std::unique_ptr<Perlin3D> noiseBlend ;

        float timeScale;
        float currentTime;
        bool movement;
        bool uniquePerlin;

        void initPerlins();
        float smoothstep(float edge0, float edge1, float x);
        


    public:


        //constructor 
        PerlinManager();
        ~PerlinManager() = default;


        void updateMesh(ofMesh &mesh );

        // ===== setters =====
        void setMovementVal(bool val);

        void setUniquePerlin(bool val);

        // ===== getters =====
        
        







};



#endif

