//
// Created by Oscar Jauffret on 13/04/2026.
//

#ifndef OCEANMANAGER_HPP
#define OCEANMANAGER_HPP

#include "shader.hpp"

class OceanManager {
private:
    std::vector<Wave> waves;

    float sampleJONSWAP(float omega, float gravity, float windSpeed195m, float alpha, float omegaP, float gamma);
public:

    void generateOcean(int waveCount, const PhysicsConfig& params);
    void uploadToShader(const Shader& shader);
};



#endif //OCEANMANAGER_HPP
