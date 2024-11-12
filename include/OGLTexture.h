#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>

class OGLTexture 
{
public:
    OGLTexture(const std::string& InPath);
    ~OGLTexture();
    void Bind(GLuint InTextureSlot);
private:
    GLuint Id;
};