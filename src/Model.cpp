#include "Model.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <algorithm>

Model::Model(const std::string& objPath, const std::string& mtlPath) {
    loadOBJ(objPath);
    loadMTL(mtlPath);
    processVertexData();

    // Load textures
    for (auto& [name, material] : materials) {
        if (!material.diffuseTexture.empty()) {
            std::string texturePath = "assets/" + material.diffuseTexture;
            materialTextures[name] = TextureManager::LoadTexture(texturePath);
        }
    }

    // Create VAOs/VBOs
    for (const auto& [name, data] : materialVertexData) {
        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Normals
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        VAOs[name] = VAO;
        VBOs[name] = VBO;
    }
}

Model::~Model() {
    // Cleanup textures
    for (auto& [name, textureID] : materialTextures) {
        TextureManager::DeleteTexture(textureID);
    }
    
    // Cleanup VAOs/VBOs
    for (auto& [name, VAO] : VAOs) {
        glDeleteVertexArrays(1, &VAO);
    }
    for (auto& [name, VBO] : VBOs) {
        glDeleteBuffers(1, &VBO);
    }
}

void Model::Draw(Shader& shader) {
    shader.use();
    for (const auto& [name, data] : materialVertexData) {
        const auto& material = materials[name];
        GLuint textureID = materialTextures[name];

        // Set material properties
        shader.setVec3("material.ambient", glm::vec3(material.Ka[0], material.Ka[1], material.Ka[2]));
        shader.setVec3("material.diffuse", glm::vec3(material.Kd[0], material.Kd[1], material.Kd[2]));
        shader.setVec3("material.specular", glm::vec3(material.Ks[0], material.Ks[1], material.Ks[2]));
        shader.setFloat("material.shininess", 32.0f);

        // Bind texture if available
        if (textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            shader.setInt("diffuseMap", 0);
        }

        // Draw mesh
        glBindVertexArray(VAOs[name]);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(data.size() / 8));
        glBindVertexArray(0);
    }
}

void Model::loadOBJ(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line, currentMaterial;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            Vertex v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (type == "vt") {
            TexCoord vt;
            iss >> vt.u >> vt.v;
            texCoords.push_back(vt);
        }
        else if (type == "vn") {
            Normal vn;
            iss >> vn.x >> vn.y >> vn.z;
            normals.push_back(vn);
        }
        else if (type == "usemtl") {
            iss >> currentMaterial;
        }
        else if (type == "f") {
            Face face;
            for (int i = 0; i < 3; ++i) {
                std::string vertexData;
                iss >> vertexData;
                std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                std::istringstream vStream(vertexData);

                int v, vt, vn;
                vStream >> v >> vt >> vn;
                face.vertexIndices[i] = v - 1;
                face.texCoordIndices[i] = vt - 1;
                face.normalIndices[i] = vn - 1;
            }
            face.materialName = currentMaterial;
            faces.push_back(face);
        }
    }
}

void Model::loadMTL(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    Material currentMaterial;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "newmtl") {
            if (!currentMaterial.name.empty()) {
                materials[currentMaterial.name] = currentMaterial;
            }
            currentMaterial = Material();
            iss >> currentMaterial.name;
        }
        else if (type == "Ka") {
            iss >> currentMaterial.Ka[0] >> currentMaterial.Ka[1] >> currentMaterial.Ka[2];
        }
        else if (type == "Kd") {
            iss >> currentMaterial.Kd[0] >> currentMaterial.Kd[1] >> currentMaterial.Kd[2];
        }
        else if (type == "Ks") {
            iss >> currentMaterial.Ks[0] >> currentMaterial.Ks[1] >> currentMaterial.Ks[2];
        }
        else if (type == "map_Kd") {
            iss >> currentMaterial.diffuseTexture;
        }
    }
    if (!currentMaterial.name.empty()) {
        materials[currentMaterial.name] = currentMaterial;
    }
}

void Model::processVertexData() {
    for (const Face& face : faces) {
        const std::string& matName = face.materialName;
        for (int i = 0; i < 3; ++i) {
            const Vertex& v = vertices[face.vertexIndices[i]];
            const TexCoord& t = texCoords[face.texCoordIndices[i]];
            const Normal& n = normals[face.normalIndices[i]];

            materialVertexData[matName].insert(materialVertexData[matName].end(), {
                v.x, v.y, v.z,        // Position
                t.u, t.v,             // Texture coordinates
                n.x, n.y, n.z         // Normal
            });
        }
    }
}