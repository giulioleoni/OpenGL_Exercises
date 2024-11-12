#version 460 core

layout (binding = 0) uniform sampler2D light_tex;

in vec2 vert_uv_out;

out vec4 frag_color;

void main() 
{
    frag_color = texture(light_tex, vert_uv_out);
}
