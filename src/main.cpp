#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "window.hpp"
#include "mesh.hpp"

int main() {
    Window window(800, 600, "OceanSim");
    Shader shader("../shaders/base.vert", "../shaders/base.frag");
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

    Mesh mesh(200);



    // THE RENDER LOOP
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!window.shouldClose()) {

        // 2. Clear the screen (The "Blank Canvas")
        window.clear(0.2f, 0.3f, 0.3f, 1.0f);

        // 3. Draw the Triangle
        shader.use();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.setFloat("uTime", static_cast<float>(glfwGetTime()));

        // Wave 1: The Base Swell (Long, slow, low frequency)
        // Large wavelength, moderate amplitude, slow speed.
        shader.setWave("waves[0]", glm::vec2(1.0f, 0.2f), 0.4f, 25.0f, 1.5f);

        // Wave 2: The Secondary Chop (Moves at an angle)
        // Half the wavelength, lower amplitude, faster.
        shader.setWave("waves[1]", glm::vec2(0.4f, 0.8f), 0.15f, 12.0f, 2.8f);

        // Wave 3: Surface Ripples (High frequency noise)
        // Very short wavelength, tiny amplitude, very fast.
        shader.setWave("waves[2]", glm::vec2(-0.2f, 0.9f), 0.05f, 4.0f, 5.0f);

        mesh.draw();

        // 4. Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }
    glfwTerminate();

    //glfwTerminate();
    return 0;
}