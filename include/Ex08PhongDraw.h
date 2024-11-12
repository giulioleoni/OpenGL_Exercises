#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
class Ex08PhongDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    GLuint Vao;
    GLuint Vbo;

    OGLProgram* Program;
    OGLTexture* StormTexture;

    glm::mat4 View;
    glm::mat4 Projection;

    size_t VerticeCount;
};