//
// Created by Oscar Jauffret on 11/04/2026.
//
#include "Mesh.hpp"

Mesh::Mesh(int resolution, float size) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // 1. Generate Vertices
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            float x = (float)i / (float)resolution * size - size/2.0f;
            float z = (float)j / (float)resolution * size - size/2.0f;

            vertices.push_back(x);    // X
            vertices.push_back(0.0f); // Y (Water surface)
            vertices.push_back(z);    // Z
        }
    }

    // 2. Generate Indices (The "Stitching")
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            int row1 = i * (resolution + 1) + j;
            int row2 = (i + 1) * (resolution + 1) + j;

            // Triangle 1
            indices.push_back(row1);
            indices.push_back(row1 + 1);
            indices.push_back(row2);

            // Triangle 2
            indices.push_back(row1 + 1);
            indices.push_back(row2 + 1);
            indices.push_back(row2);
        }
    }
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