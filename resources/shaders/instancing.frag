#version 460 core

out vec4 frag_color;

layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D wood_tex;

in vec2 vert_uv_out;
in flat int inst_id_out;

void main() 
{
    /*
    vec4 color;
    if (inst_id_out % 2 == 0) 
    {
        color = texture(smile_tex, vert_uv_out);
    } 
    else 
    {
        color = texture(wood_tex, vert_uv_out);
    }
    */


    //frag_color = vec4(1.0f, 0.f, 0.f, 1.f);
    //frag_color = color;

    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 wood_texel = texture(wood_tex, vert_uv_out);
    frag_color = mix(smile_texel, wood_texel, inst_id_out % 2);

}