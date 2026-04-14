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
#include "oceanManager.hpp"
#include "camera.hpp"

// Global so the GLFW callback can reach it
Camera camera(glm::vec3(0.0f, 10.0f, 15.0f));

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    camera.processMouse(xpos, ypos);
}
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
        config.physics.fetch = j["physics"]["fetch"];
        config.physics.windDirection = j["physics"]["windDirection"];
        config.physics.windSpread = j["physics"]["windSpread"];
        config.physics.windSpeed10m = j["physics"]["windSpeed10m"];
        config.physics.windSpeed195m = j["physics"]["windSpeed195m"];

        config.shaders.vertex = j["shaders"]["vertex"];
        config.shaders.fragment = j["shaders"]["fragment"];
    }
};

int main() {
    ConfigManager manager;
    OceanManager oceanManager;
    manager.load("../config.json");

    const Config& cfg = manager.config;
    oceanManager.generateOcean(64, cfg.physics);

    Window window(cfg.window.width, cfg.window.height, cfg.window.title);

    // Capture mouse
    glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.getHandle(), mouseCallback);

    Shader shader(cfg.shaders.vertex.c_str(), cfg.shaders.fragment.c_str());
    shader.use();


    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(cfg.window.width) / static_cast<float>(cfg.window.height),
        0.1f,
        100.0f
    );

    Mesh mesh(cfg.ocean.resolution);

    double lastTime = glfwGetTime();
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    int nbFrames = 0;

    if (cfg.ocean.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    while (!window.shouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        camera.processKeyboard(window.getHandle(), deltaTime);

        // Escape to release mouse / close
        if (glfwGetKey(window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window.getHandle(), true);

        window.clear(1.0f, 1.0f, 1.0f, 1.0f);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("model", model);
        shader.setFloat("uTime", currentFrame);
        shader.setVec3("viewPos", camera.position);

        shader.setVec3("light.direction", glm::normalize(glm::vec3(0.0f, 0.6f, -0.8f)));
        shader.setVec3("light.color", glm::vec3(1.0f, 0.9f, 0.8f));

        shader.setVec3("deepColor", glm::vec3(0.01f, 0.04f, 0.08f));
        shader.setVec3("shallowColor", glm::vec3(0.0f, 0.15f, 0.15f));

        oceanManager.uploadToShader(shader);
        mesh.draw();

        window.swapBuffers();
        window.pollEvents();

        nbFrames++;
        double now = glfwGetTime();
        if (now - lastTime >= 1.0) {
            std::string title = "Ocean Simulation | FPS: " + std::to_string(nbFrames);
            glfwSetWindowTitle(window.getHandle(), title.c_str());
            nbFrames = 0;
            lastTime += 1.0;
        }
    }

    glfwTerminate();

    //glfwTerminate();
    return 0;
}