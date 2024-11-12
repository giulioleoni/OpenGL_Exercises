#version 460 core

out vec4 frag_color;

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D wood_box;

void main() 
{
    vec4 wood_texel = texture(wood_box, vert_uv_out);
    frag_color = wood_texel;
}