#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "OGLTexture.h"


struct QuadInstance {
    glm::vec3 Position;
    float RotationSpeed;
};

class Ex09InstancingDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    GLuint MvpVbo;
    OGLProgram* Program;
    glm::mat4 View;
    glm::mat4 Projection;

    std::vector<QuadInstance> Quads;
    std::vector<glm::mat4> MvpData;

    OGLTexture* SmileTexture;
    OGLTexture* BoxTexture;
};


