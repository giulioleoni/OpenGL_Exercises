#include "Ex04QuadColorDraw.h"
#include <string>
#include <vector>
#include "OGLProgram.h"
#include <cmath>

void Ex04QuadColorDraw::Start() {

    Program = new OGLProgram("resources/shaders/quadcolor.vert", "resources/shaders/quadcolor.frag");

    //Counter-Clock wise. By now vertex data in NDC space
    std::vector<float> Vertices = {
        //Position          //Colors
        0.5f, -0.5f, 0.f,   1.f, 0.f, 0.f,  //bottom-right
       -0.5f, -0.5f, 0.f,   0.f, 1.f, 0.f,  //bottom-left
        0.5f,  0.5f, 0.f,   0.f, 0.f, 1.f,  //top-right
       -0.5f,  0.5f, 0.f,   1.f, 1.f, 0.f   //top-left
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Create EBO (element buffer)
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(uint32_t), Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind(); 
\
    GLint BaseColorLocation = glGetUniformLocation(Program->GetId(), "base_color");
    Color ColorVar{0.5f, 0.5f, 0.5f, 1.f};
    const GLfloat* RedPtr = reinterpret_cast<GLfloat*>(&ColorVar);
    glUniform4fv(BaseColorLocation, 1, RedPtr);
}

void Ex04QuadColorDraw::Update(float InDeltaTime) {
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    glClear(GL_COLOR_BUFFER_BIT);
    
    Color TimedColor;  //0, 1
    float value = sinf(ElapsedTime) * 0.5f + 0.5f; //-1, 1
    TimedColor.R = value;
    TimedColor.G = cosf(ElapsedTime)* 0.5f + 0.5f;
    TimedColor.B = cosf(ElapsedTime + 1.1f) * 0.5f + 0.5f;
    TimedColor.A = 1.f;

    glUniform4fv(glGetUniformLocation(Program->GetId(), "base_color"), 1, reinterpret_cast<GLfloat*>(&TimedColor));

    float scaleSpeed = 10.f;
    float scale = sinf(ElapsedTime * scaleSpeed) * 0.5f + 0.5f;
    glUniform1f(glGetUniformLocation(Program->GetId(), "scale"), scale);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Ex04QuadColorDraw::Destroy() {
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);

    delete Program;
}

