//
// Created by Oscar Jauffret on 13/04/2026.
//

#include "../include/oceanManager.hpp"


float OceanManager::sampleJONSWAP(float omega, float gravity, float windSpeed195m, float alpha, float omegaP, float gamma) {
    float omega0 = gravity / windSpeed195m;
    float beta = 0.74f;

    float pm = (alpha * gravity * gravity) / pow(omega, 5.0f) * exp(-beta * pow(omega0 / omega, 4.0f));

    float sigma = (omega <= omegaP) ? 0.07f : 0.09f;
    float numerator = pow(omega - omegaP, 2.0f);
    float denominator = 2.0f * sigma * sigma * omegaP * omegaP;
    float r = exp(-numerator / denominator);

    return pm * pow(gamma, r);
}


void OceanManager::generateOcean(int waveCount, const PhysicsConfig& params) {
    float windSpeed10m = params.windSpeed10m;
    float windSpeed195m = params.windSpeed195m;
    float windDirection = params.windDirection;
    float fetch = params.fetch;
    float gravity = params.gravity;
    float windSpread = params.windSpread;

    float alpha = 0.076 * pow(windSpeed10m * windSpeed10m / (fetch * gravity), 0.22);
    float omegaP = 22.0 * pow(gravity * gravity / (windSpeed10m * fetch), 0.33);
    float gamma = 3.3f;

    waves.clear();

    float omegaMin = 0.5f * omegaP;
    float omegaMax = 5.0f * omegaP;
    float deltaOmega = (omegaMax - omegaMin) / waveCount;

    for (int i = 0; i < waveCount; i++) {
        float currentOmega = omegaMin + (i * deltaOmega);
        float s = sampleJONSWAP(currentOmega, gravity, windSpeed195m, alpha, omegaP, gamma);
        float h = sqrt(2.0f * s * deltaOmega);

        float randomOffset = ((float)rand() / RAND_MAX - 0.5f) * windSpread;
        float theta = windDirection + randomOffset;

        waves.emplace_back(theta, currentOmega, h, gravity);
    }
}

void OceanManager::uploadToShader(const Shader& shader) {
    shader.setFloat("numberOfWaves", static_cast<float>(waves.size()));
    int waveIndex = 0;
    for (const auto &wave : waves) {
        std::string baseName = "waves[" + std::to_string(waveIndex++) + "]";
        shader.setWave(baseName, wave);
    }
}
