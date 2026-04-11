//
// Created by Oscar Jauffret on 11/04/2026.
//
#include "Mesh.hpp"

Mesh::Mesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    vertices = {
        0.5, 0.5, 0.0,   // top right
        0.5, -0.5, 0.0,  // bottom right
        -0.5, 0.5, 0.0,    // top left
        -0.5, -0.5, 0.0   // bottom left
    };

    indices = {
        0, 1, 2,  // first triangle
        2, 3, 1   // second triangle
    };
    m_indexCount = indices.size();
    setupMesh(vertices, indices);
}

void Mesh::setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // use current VAO for all subsequent calls

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Layout 0: Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}