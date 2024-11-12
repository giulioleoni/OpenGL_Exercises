#version 460 core

out vec4 frag_color;

//in vec4 vert_color_out;

layout (location = 0) in vec4 PIPPO;

uniform vec4 base_color;

void main() 
{
    //frag_color = vec4(1.0f, 0.f, 0.f, 1.f);
    //frag_color = vert_color_out;
    frag_color = vert_color_out * base_color;
}