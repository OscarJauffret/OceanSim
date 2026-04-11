//
// Created by Oscar Jauffret on 11/04/2026.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <vector>

class Mesh {
public:
    // The constructor generates the grid automatically
    explicit Mesh(int resolution);
    ~Mesh();

    // The single call to draw the entire ocean
    void draw() const;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int m_indexCount;

    void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
};


#endif //MESH_HPP
