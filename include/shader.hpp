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

class Shader {
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();

        void use();
        void setFloat(const std::string &name, float value) const;
        void del() const;
};


#endif //SHADER_HPP
