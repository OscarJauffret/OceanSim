#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.hpp"
#include "window.hpp"
#include "mesh.hpp"

int main() {
    Window window(800, 600, "OceanSim");
    Shader shader("../shaders/base.vert", "../shaders/base.frag");
    shader.use();

    Mesh mesh;

    // THE RENDER LOOP
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!window.shouldClose())
    {
        // 1. Handle Input
        if (glfwGetKey(window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window.getHandle(), true);

        // 2. Clear the screen (The "Blank Canvas")
        window.clear(0.2f, 0.3f, 0.3f, 1.0f);

        // 3. Draw the Triangle
        shader.use();
        mesh.draw();

        // 4. Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }
    glfwTerminate();

    //glfwTerminate();
    return 0;
}