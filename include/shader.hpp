//
// Created by Oscar Jauffret on 11/04/2026.
//

#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <glm/glm.hpp>
#include "oceanConfig.hpp"

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
        void setVec3(const std::string &name, const glm::vec3 &vec) const;
        void setWave(const std::string &name, Wave wave) const;
};


#endif //SHADER_HPP
