//
// Created by Oscar Jauffret on 11/04/2026.
//

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    Window(int width, int height, std::string title);
    ~Window();

    bool shouldClose();
    void swapBuffers();
    void pollEvents();
    void clear(float r, float g, float b, float a);

    GLFWwindow* getHandle() { return m_window; }

private:
    GLFWwindow* m_window;
    int m_width, m_height;
};



#endif //WINDOW_HPP
