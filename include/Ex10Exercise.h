#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
class Ex10Exercise 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    GLuint Vao;
    GLuint Vbo;

    GLuint VaoLight;
    GLuint VboLight;

    OGLProgram* Program;
    OGLProgram* ProgramLight;
    OGLTexture* StormTexture;
    OGLTexture* LightTexture;

    glm::mat4 View;
    glm::mat4 Projection;

    glm::vec3 StormPosition;

    size_t VerticeCount;
};