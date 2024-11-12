#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_color;

out vec4 vert_color_out;

uniform float scale;

void main() 
{   
    //gl_Position = vec4(vert_pos, 1.f);

    vec3 vert_pos_scaled = vert_pos * scale;
    gl_Position = vec4(vert_pos_scaled, 1.f);
    vert_color_out = vec4(vert_color, 1.f);
}