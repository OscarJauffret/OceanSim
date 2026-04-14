//
// Created by Oscar Jauffret on 11/04/2026.
//

#ifndef OCEANCONFIG_HPP
#define OCEANCONFIG_HPP
#include <glm/glm.hpp>

struct Wave {
    Wave(float theta, float omega, float amplitude, float gravity) : theta(theta), amplitude(amplitude), omega(omega) {
        waveNumber = omega * omega / gravity;
        wavelength = 2.0f * 3.14159265f / waveNumber;
    }

    float theta;
    float amplitude;
    float wavelength;

    float waveNumber;
    float omega;        // angular frequency (sqrt(g * k))
};

struct OceanConfig {
    // General
    int resolution;
    float size;

    // Rendering
    glm::vec3 deepColor;
    glm::vec3 shallowColor;
    bool wireframe;

    int numberOfWaves;
};

struct Shaders {
    std::string vertex;
    std::string fragment;
};

struct WindowConfig {
    int width;
    int height;
    std::string title;
};

struct PhysicsConfig {
    float gravity;
    float windSpeed10m;     // Wind speed 10m above the surface
    float windSpeed195m;    // Wind speed 19.5m above the surface

    float windDirection; // Theta
    float windSpread;     // Random variation in wave direction (radians)

    float fetch;         // Distance wind has traveled
};

struct Config {
    WindowConfig window;
    Shaders shaders;
    OceanConfig ocean;
    PhysicsConfig physics;
};

#endif //OCEANCONFIG_HPP
