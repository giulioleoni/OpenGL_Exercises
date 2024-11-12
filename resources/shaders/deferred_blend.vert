#version 460 core

layout (location = 0) in vec2 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

void main() 
{   
    gl_Position = vec4(vert_pos, 0.f, 1.f);
    vert_uv_out = vert_uv;
}