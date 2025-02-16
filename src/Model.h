#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>  

// Structs to hold OBJ data

struct Vertex { float x, y, z; };
struct TexCoord { float u, v; };
struct Normal { float x, y, z; };

struct Face {
    int vertexIndices[3];
    int texCoordIndices[3];
    int normalIndices[3];
    std::string materialName;
};

struct Material {
    std::string name;
    float Ka[3]; // Ambient
    float Kd[3]; // Diffuse
    float Ks[3]; // Specular
    std::string diffuseTexture;
};

class Model {
private:
    std::vector<Vertex> vertices;
    std::vector<TexCoord> texCoords;
    std::vector<Normal> normals;
    std::vector<Face> faces;
    std::map<std::string, Material> materials;
    std::map<std::string, std::vector<float>> materialVertexData;
    std::map<std::string, GLuint> VAOs;
    std::map<std::string, GLuint> VBOs;
    std::map<std::string, GLuint> materialTextures;

    void loadOBJ(const std::string& filepath);
    void loadMTL(const std::string& filepath);
    void processVertexData();

public:
    Model(const std::string& objPath, const std::string& mtlPath);
    ~Model();
    void Draw(Shader& shader);
};

#endif