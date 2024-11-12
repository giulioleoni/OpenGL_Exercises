#include "Ex07CubePrespDraw.h"
#include <string>
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

void Ex07CubePrespDraw::Start() {

    Program = new OGLProgram("resources/shaders/cubepersp.vert", "resources/shaders/cubepersp.frag");

    //Counter-Clock wise. vertex data are no more in NDC space (because of the Z)
    std::vector<float> Vertices = {
        //positions      uvs
         //FRONT FACE
        -1, -1, 1,   0, 0,    //bottom-left
         1, -1, 1,   1, 0,    //bottom-right
         1,  1, 1,   1, 1,    //top-right
        -1,  1, 1,   0, 1,    //top-left 
        -1, -1, 1,   0, 0,    //bottom-left
         1,  1, 1,   1, 1,    //top-right

        //BACK FACE
         1, -1,-1,   0, 0,    //bottom-left
        -1, -1,-1,   1, 0,    //bottom-right
        -1,  1,-1,   1, 1,    //top-right
         1,  1,-1,   0, 1,    //top-left 
         1, -1,-1,   0, 0,    //bottom-left
        -1,  1,-1,   1, 1,    //top-right

         //LEFT FACE
        -1, -1,-1,   0, 0,    //bottom-left
        -1, -1, 1,   1, 0,    //bottom-right
        -1,  1, 1,   1, 1,    //top-right
        -1,  1,-1,   0, 1,    //top-left 
        -1, -1,-1,   0, 0,    //bottom-left
        -1,  1, 1,   1, 1,    //top-right

        //RIGHT FACE
         1, -1, 1,   0, 0,    //bottom-left
         1, -1,-1,   1, 0,    //bottom-right
         1,  1,-1,   1, 1,    //top-right
         1,  1, 1,   0, 1,    //top-left 
         1, -1, 1,   0, 0,    //bottom-left
         1,  1,-1,   1, 1,    //top-right

         //TOP FACE
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1, 1,   1, 0,    //bottom-right
          1, 1,-1,   1, 1,    //top-right
         -1, 1,-1,   0, 1,    //top-left 
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1,-1,   1, 1,    //top-right
         
         //BOTTOM FACE
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1,-1,   1, 0,    //bottom-right
          1,-1, 1,   1, 1,    //top-right
         -1,-1, 1,   0, 1,    //top-left 
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1, 1,   1, 1,    //top-right
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

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind(); 

    //5. Create Texture
    BoxTexture = new OGLTexture("resources/textures/wood-box.jpg");

    BoxTexture->Bind(GL_TEXTURE0);
    //glUniform1i(glGetUniformLocation(Program->GetId(), "smile_tex"), 0);
    //glUniform1i(glGetUniformLocation(Program->GetId(), "wood_box"), 1);

    //6. Enable Dept Testing
    glEnable(GL_DEPTH_TEST);

    //7. Enable Cull Face
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); //default
    glFrontFace(GL_CCW);  //default
}

void Ex07CubePrespDraw::Update(float InDeltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    Program->SetUniform("rotation", 20.f * ElapsedTime);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Ex07CubePrespDraw::Destroy() {
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete BoxTexture;
    delete Program;
}

