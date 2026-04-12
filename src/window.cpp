//
// Created by Oscar Jauffret on 11/04/2026.
//

#include "window.hpp"
using std::cerr, std::endl;

Window::Window(int width, int height, std::string title) : m_width(width), m_height(height) {
    if (!glfwInit()) {
        cerr << "Failed to init GLFW" << endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!m_window) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(m_window);

    // GLEW init MUST happen after context creation
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        exit(-1);
    }
}

Window::~Window() {
    glfwDestroyWindow(getHandle());
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwGetKey(getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

void Window::swapBuffers() {
    glfwSwapBuffers(getHandle());
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}