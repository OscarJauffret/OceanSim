//
// Created by Oscar Jauffret on 11/04/2026.
//

#ifndef OCEANCONFIG_HPP
#define OCEANCONFIG_HPP
#include <glm/glm.hpp>

struct WaveData {
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

    // Simulation
    std::vector<WaveData> waves;
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
};

struct Config {
    WindowConfig window;
    Shaders shaders;
    OceanConfig ocean;
    PhysicsConfig physics;
};

#endif //OCEANCONFIG_HPP
