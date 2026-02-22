#ifndef PERLIN_MANAGER_HPP
#define PERLIN_MANAGER_HPP


#include "../perlin3D/perlin3D.hpp"

class PerlinManager {
    private:

        std::vector<std::unique_ptr<Perlin3D>> perlins;
        
        std::unique_ptr<Perlin3D> noiseBlend ;

        float timeScale;
        float currentTime;
        int size ;
        bool movement;
        bool uniquePerlin;
        bool earth;
        bool rock;
        bool gravel;
        

        void initPerlins();
        float smoothstep(float edge0, float edge1, float x);
        


    public:


        //constructor 
        PerlinManager(int meshSize);
        ~PerlinManager() = default;


        void updateMesh(ofMesh &mesh );
        void earthMesh(ofMesh &mesh);

        // ===== setters =====
        void setMovementVal(bool val);
        void setRockVal(bool val);
        void setEarthVal(bool val);
        void setCaveVal(bool val);


        // ===== getters =====
        
        







};



#endif

