//
// Created by Oscar Jauffret on 11/04/2026.
//

#include "shader.hpp"

int check_shader_compilation(unsigned int shader, const char* shaderType = "DEFAULT") {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog;
    } else {
        std::cout << "Success:: SHADER::" << shaderType << "::COMPILATION_PASSED" << std::endl;
    }
    return success;
}


int check_program_linking(unsigned int program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog;
    } else {
        std::cout << "Success:: PROGRAM::LINKING_PASSED" << std::endl;
    }
    return success;
}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. Retrieve the source code from filePaths
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderSource = vertexCode.c_str();
    const char* fShaderSource = fragmentCode.c_str();

    // 2. Compile Shaders (Use your existing check_shader_compilation logic here)
    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderSource, NULL);
    glCompileShader(vertex);
    check_shader_compilation(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderSource, NULL);
    glCompileShader(fragment);
    check_shader_compilation(fragment, "FRAGMENT");

    // 3. Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    check_program_linking(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    del();
}

void Shader::del() const {
    glDeleteProgram(ID);
}

void Shader::use() { glUseProgram(ID); }

// Helper for Uniforms (Crucial for waves!)
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
}

void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
}

void Shader::setWave(const std::string& name, glm::vec2 dir, float amplitude, float wavelength, float speed) const {
    glm::vec2 unitDir = glm::normalize(dir);
    setVec2(name + ".direction", unitDir);
    setFloat(name + ".amplitude", amplitude);
    setFloat(name + ".wavelength", wavelength);
    setFloat(name + ".speed", speed);
}
