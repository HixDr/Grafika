#include "Sphere.h"
#include <cmath>

const float PI = 3.14159265359f;

Sphere::Sphere(unsigned int xSegments, unsigned int ySegments) {
    for (unsigned int y = 0; y <= ySegments; ++y) {
        for (unsigned int x = 0; x <= xSegments; ++x) {
            float xSegment = (float)x / xSegments;
            float ySegment = (float)y / ySegments;
            
            Vertex vertex;
            vertex.Position.x = std::cos(xSegment * 2 * PI) * std::sin(ySegment * PI);
            vertex.Position.y = std::cos(ySegment * PI);
            vertex.Position.z = std::sin(xSegment * 2 * PI) * std::sin(ySegment * PI);
            vertex.Normal = vertex.Position;
            vertex.TexCoords = glm::vec2(xSegment, ySegment);
            
            vertices.push_back(vertex);
        }
    }

    for (unsigned int y = 0; y < ySegments; ++y) {
        for (unsigned int x = 0; x < xSegments; ++x) {
            Indices.push_back((y + 1) * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x + 1);
            Indices.push_back((y + 1) * (xSegments + 1) + x);
            Indices.push_back(y * (xSegments + 1) + x + 1);
            Indices.push_back((y + 1) * (xSegments + 1) + x + 1);
        }
    }
}

void Sphere::setupSphere() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
}

void Sphere::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}