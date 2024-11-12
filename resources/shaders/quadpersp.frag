#version 460 core

out vec4 frag_color;

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D wood_box;

//uniform sampler2D smile_tex;
//uniform sampler2D wood_box;

void main() 
{
    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 wood_texel = texture(wood_box, vert_uv_out);
    frag_color = mix(smile_texel, wood_texel, 0.5f);
}