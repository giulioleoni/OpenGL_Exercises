#pragma once
#include "glad/glad.h"

class Ex01TriangleDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    GLuint Vao;
    GLuint Vbo;
    GLuint ProgramId;
};