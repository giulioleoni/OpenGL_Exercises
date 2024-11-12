#include "Ex08PhongDraw.h"
#include <string>
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#define OBJ_IMPLEMENTATION
#include "obj_parser.h"


void Ex08PhongDraw::Start() {

    Program = new OGLProgram("resources/shaders/phong.vert", "resources/shaders/phong.frag");

    obj_t* mesh = obj_parse("resources/models/stormtrooper.obj");
    
    std::vector<float> Vertices;       
    for(int i=0; i < mesh->triangle_count; ++i) {
        obj_triangle_t& t = mesh->triangles[i];

        Vertices.push_back(t.v1.position.x);
        Vertices.push_back(t.v1.position.y);
        Vertices.push_back(t.v1.position.z);
        Vertices.push_back(t.v1.uv.x);
        Vertices.push_back(t.v1.uv.y);
        Vertices.push_back(t.v1.normal.x);
        Vertices.push_back(t.v1.normal.y);
        Vertices.push_back(t.v1.normal.z);

        Vertices.push_back(t.v2.position.x);
        Vertices.push_back(t.v2.position.y);
        Vertices.push_back(t.v2.position.z);
        Vertices.push_back(t.v2.uv.x);
        Vertices.push_back(t.v2.uv.y);
        Vertices.push_back(t.v2.normal.x);
        Vertices.push_back(t.v2.normal.y);
        Vertices.push_back(t.v2.normal.z);

        Vertices.push_back(t.v3.position.x);
        Vertices.push_back(t.v3.position.y);
        Vertices.push_back(t.v3.position.z);
        Vertices.push_back(t.v3.uv.x);
        Vertices.push_back(t.v3.uv.y);
        Vertices.push_back(t.v3.normal.x);
        Vertices.push_back(t.v3.normal.y);
        Vertices.push_back(t.v3.normal.z);
    }
    VerticeCount = mesh->triangle_count * 3;
    obj_parse_destroy(mesh);

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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind(); 

    //5. Create Texture
    StormTexture = new OGLTexture("resources/models/stormtrooper.png");
    StormTexture->Bind(GL_TEXTURE0);
    
    //6. Enable Dept Testing
    glEnable(GL_DEPTH_TEST);

    //7. Enable Cull Face
    glEnable(GL_CULL_FACE);

    //Camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60.f;
    float AspectRatio = 800.f / 600.f;
    float ZNear = 0.01f;
    float ZFar = 100.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);

    Program->SetUniform("camera_pos", Position);

    //Light
    glm::vec3 PointLightPos = glm::vec3(4, 0, 0);
    Program->SetUniform("point_light_pos", PointLightPos);
}

void Ex08PhongDraw::Update(float InDeltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    float Angle = 20.f * ElapsedTime;

    //Model = Translate * Rotate * Scale
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    /* Alternative preparing indipendent matrices
    glm::mat4 Identity =  glm::mat4(1.f);
    glm::mat4 Transl = glm::translate(Identity, glm::vec3(0, -4, 0));
    glm::mat4 Rotate = glm::rotate(Identity, glm::radians(Angle), glm::vec3(0, 1, 0));
    glm::mat4 Scale  = glm::scale(Identity, glm::vec3(2.f));
    glm::mat4 Model = Transl * Rotate * Scale;
    */

    glm::mat4 Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);

    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);
}

void Ex08PhongDraw::Destroy() {
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete StormTexture;
    delete Program;
}

