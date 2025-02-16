#pragma once
#include <GL/glew.h>
#include <string>

class TextureManager {
public:
    static GLuint LoadTexture(const std::string& filepath);
    static void DeleteTexture(GLuint textureID);
};