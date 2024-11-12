#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"

class Ex04QuadColorDraw 
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
};