//
// Created by Oscar Jauffret on 14/04/2026.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw   = -90.0f;
    float pitch  = -30.0f;
    float speed  = 10.0f;
    float sensitivity = 0.1f;

    bool firstMouse = true;
    double lastX = 400.0;
    double lastY = 300.0;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 10.0f, 15.0f))
        : position(pos), worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
    {
        updateVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(GLFWwindow* window, float dt) {
        float velocity = speed * dt;

        // ZQSD — movement relative to where you're looking (horizontal plane)
        glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 flatRight = right;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += flatFront * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= flatFront * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= flatRight * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += flatRight * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) position.y += velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) position.y -= velocity;
    }

    void processMouse(double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX) * sensitivity;
        float yoffset = static_cast<float>(lastY - ypos) * sensitivity; // inverted Y
        lastX = xpos;
        lastY = ypos;

        yaw   += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)  pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateVectors();
    }

private:
    void updateVectors() {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }
};

#endif //CAMERA_HPP
