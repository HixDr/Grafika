#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"

class Sphere {
    struct Vertex { glm::vec3 Position, Normal; glm::vec2 TexCoords; };
public:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> Indices;
    
    Sphere(unsigned int xSegments, unsigned int ySegments);
    void Draw();
    void setupSphere();
};

#endif