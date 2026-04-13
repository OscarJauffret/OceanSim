#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "window.hpp"
#include "mesh.hpp"
#include "oceanConfig.hpp"
#include "json.hpp"

class ConfigManager {
public:
    Config config;

    void load(const std::string& path) {
        std::ifstream file(path);
        nlohmann::json j;
        file >> j;

        config.window.width = j["window"]["width"];
        config.window.height = j["window"]["height"];
        config.window.title = j["window"]["title"].get<std::string>();

        config.ocean.resolution = j["ocean"]["resolution"];
        config.ocean.size = j["ocean"]["size"];
        config.ocean.wireframe = j["ocean"]["wireframe"];

        config.physics.gravity = j["physics"]["gravity"];

        config.shaders.vertex = j["shaders"]["vertex"];
        config.shaders.fragment = j["shaders"]["fragment"];

        config.ocean.waves.clear();
        for (auto& w : j["waves"]) {
            WaveData wave{};
            wave.theta = w["theta"];
            wave.amplitude = w["amp"];
            wave.wavelength = w["len"];

            wave.waveNumber = 2.0f * 3.14159265f / wave.wavelength;
            wave.omega = sqrt(config.physics.gravity * wave.waveNumber);

            config.ocean.waves.push_back(wave);
        }
    }
};

int main() {
    ConfigManager manager;
    manager.load("../config.json");
    const Config& cfg = manager.config;

    Window window(cfg.window.width, cfg.window.height, cfg.window.title);
    Shader shader(cfg.shaders.vertex.c_str(), cfg.shaders.fragment.c_str());
    shader.use();

    // 1. Model Matrix: The "Identity" matrix (no transformation)
    // This keeps the ocean at (0,0,0) with no rotation or scaling.
    glm::mat4 model = glm::mat4(1.0f);

    // 2. View Matrix: The "Camera"
    // Parameters: (Camera Position, Where to look, Which way is Up)
    // We move the camera up 3 units and back 6 units to look down at the grid.
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 10.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // 3. Projection Matrix: Perspective
    // Parameters: (Field of View, Aspect Ratio, Near Clip, Far Clip)
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    Mesh mesh(cfg.ocean.resolution);



    // THE RENDER LOOP
    if (cfg.ocean.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    while (!window.shouldClose()) {

        // 2. Clear the screen (The "Blank Canvas")
        window.clear(1.0f, 1.0f, 1.0f, 1.0f);

        // 3. Draw the Triangle
        shader.use();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setFloat("uTime", static_cast<float>(glfwGetTime()));
        shader.setFloat("numberOfWaves", static_cast<float>(cfg.ocean.waves.size()));

        shader.setVec3("light.direction", glm::normalize(glm::vec3(0.5f, 1.0f, 0.3f)));
        shader.setVec3("light.color", glm::vec3(1.0f, 0.9f, 0.8f)); // Warm sunlight

        int waveIndex = 0;
        for (auto &wave : cfg.ocean.waves) {
            std::string baseName = "waves[" + std::to_string(waveIndex++) + "]";
            shader.setWave(baseName, wave);
        }
        mesh.draw();

        // 4. Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }
    glfwTerminate();

    //glfwTerminate();
    return 0;
}