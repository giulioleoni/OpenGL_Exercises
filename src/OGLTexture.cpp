#include "OGLTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include "Common.h"

GLuint CreateTexture(const std::string& InFilePath) 
{
    stbi_set_flip_vertically_on_load(true);

    int Width, Height, Channels;
    unsigned char* Data = stbi_load(InFilePath.c_str(), &Width, &Height, &Channels, 0);
    DIE_ON_NULL(Data, "Error Reading image");

    GLenum Format = Channels == 3 ? GL_RGB : GL_RGBA;

    GLuint TextureId;
    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_2D, TextureId);

    //Load Data To GPU
    glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);

    //Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //MipMapping (Optional)
    glGenerateMipmap(GL_TEXTURE_2D);

    return TextureId;
}


OGLTexture::OGLTexture(const std::string& InFilePath) 
{
    Id = CreateTexture(InFilePath);
}

OGLTexture::~OGLTexture()
{
    glDeleteTextures(1, &Id);
}

void OGLTexture::Bind(GLuint InTextureSlot)
{
    glActiveTexture(InTextureSlot);
    glBindTexture(GL_TEXTURE_2D, Id);
}