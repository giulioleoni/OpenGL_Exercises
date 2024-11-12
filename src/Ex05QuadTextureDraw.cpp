#include "Ex05QuadTextureDraw.h"
#include <string>
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

void Ex05QuadTextureDraw::Start() {

    Program = new OGLProgram("resources/shaders/quadtexture.vert", "resources/shaders/quadtexture.frag");

    //Counter-Clock wise. By now vertex data in NDC space
    std::vector<float> Vertices = {
        //Position          //UVs
        0.5f, -0.5f, 0.f,   1.f, 0.f,  //bottom-right
       -0.5f, -0.5f, 0.f,   0.f, 0.f,  //bottom-left
        0.5f,  0.5f, 0.f,   1.f, 1.f,  //top-right
       -0.5f,  0.5f, 0.f,   0.f, 1.f,  //top-left
    };

    std::vector<uint32_t> Indexes = {
        0, 1, 2,  //Triangle Right
        2, 3, 1   //Triangle Left
    };


    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Create EBO (element buffer)
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(uint32_t), Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind(); 

    Program->SetUniform("base_color", {0.5f, 0.5f, 0.5f, 1.f});

    //6. Create Texture
    SmileTexture = new OGLTexture("resources/textures/smile.png");
    BoxTexture = new OGLTexture("resources/textures/wood-box.jpg");

    SmileTexture->Bind(GL_TEXTURE0);
    BoxTexture->Bind(GL_TEXTURE1);
    //glUniform1i(glGetUniformLocation(Program->GetId(), "smile_tex"), 0);
    //glUniform1i(glGetUniformLocation(Program->GetId(), "wood_box"), 1);


    //7. Enable Alpha Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Ex05QuadTextureDraw::Update(float InDeltaTime) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Ex05QuadTextureDraw::Destroy() {
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    delete SmileTexture;
    delete BoxTexture;
    delete Program;
}

