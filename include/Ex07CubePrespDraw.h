#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"

class Ex07CubePrespDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    GLuint Vao;
    GLuint Vbo;

    OGLProgram* Program;
    OGLTexture* BoxTexture;
};