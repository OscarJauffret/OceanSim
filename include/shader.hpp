//
// Created by Oscar Jauffret on 11/04/2026.
//

#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();

        void use();
        void del() const;

        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void setVec2(const std::string &name, const glm::vec2 &vec) const;
        void setWave(const std::string &name, glm::vec2 dir, float amplitude, float waveNumber, float speed) const;
};


#endif //SHADER_HPP
