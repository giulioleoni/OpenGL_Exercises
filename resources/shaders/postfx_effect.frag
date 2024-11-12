#version 460 core

out vec4 frag_color;

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D scene_tex;

uniform float time;

//TODO: Pass Screen Width and Heigh as uniform

vec4 blur() {

    vec2 textSize = textureSize(scene_tex, 0);
    float uv_w = 1.f / textSize.x;
    float uv_h = 1.f / textSize.y;

    vec4 contrib = vec4(0.f);
    
    //taking into account 3x3 pixel matrix around current pixel
    for(float i=-1.f; i <= 1.f; ++i) 
    {
        float xoff = uv_w * i;
        for(float j=-1.f; j <= 1.f; ++j) 
        {
            float yoff = uv_h * j;
            vec2 calc_uv = vec2(vert_uv_out.x + xoff, vert_uv_out.y + yoff);
            contrib += texture(scene_tex, calc_uv);
        }        
    }

    vec4 color = contrib / 9;
    color.a = 1.f;
    return color;
}

vec4 wave() {
    //y = A sin(B(x + C)) + D
    float A = 1.f / 70.f;
    float B = 20.f;
    float C = time / 40.f;
    float D = 0.f; 
    vec2 uv_curr = vert_uv_out;
    uv_curr.x += A * sin(B * (uv_curr.y + C)) + D;
    return texture(scene_tex, uv_curr);
}

vec4 quake() {
    vec2 uv_new = vert_uv_out;
    float strength = 0.01f;

    uv_new.x += cos(time * 30) * strength;
    uv_new.y += cos(time * 45) * strength;

    return texture(scene_tex, uv_new);
}

void main() 
{
    //AS IS
    //frag_color = texture(scene_tex, vert_uv_out);

    /* Grayscale base
    vec4 color = texture(scene_tex, vert_uv_out);
    float avg = (color.r + color.g + color.b) / 3.0;
    frag_color = vec4(avg, avg, avg, color.a);
    */ 
    
    /* Graysale weighted 
    vec4 color = texture(scene_tex, vert_uv_out);
    float avgw = (0.3f * color.r + 0.59f * color.g + 0.11f * color.b);
    frag_color = vec4(avgw, avgw, avgw, color.a);
    */

    /* NEGATIVE 
    vec4 color = texture(scene_tex, vert_uv_out);
    frag_color = 1.f - color;
    */

    /* BLACKBAND with gl_FragCoord
    if (gl_FragCoord.y < 30 || gl_FragCoord.y > 370) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_tex, vert_uv_out);
    }
    */

    /* BLACKBAND with uv  
    if (vert_uv_out.y < 0.1 || vert_uv_out.y > 0.9) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_tex, vert_uv_out);
    }
    */

    /* BLUR 
    frag_color = blur();
    */

    /* WAVE 
    frag_color = wave();
    */
    
    /* QUAKE */
    frag_color = quake();

}