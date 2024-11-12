#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"

class Ex03QuadIndexDraw 
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