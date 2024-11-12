#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"

class Ex05QuadTextureDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    OGLProgram* Program;
    OGLTexture* SmileTexture;
    OGLTexture* BoxTexture;

};