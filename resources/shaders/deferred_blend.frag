#version 460 core

out vec4 frag_color;

layout (binding = 0) uniform sampler2D diffuse_tex;
layout (binding = 1) uniform sampler2D normal_tex;
layout (binding = 2) uniform sampler2D position_tex;

in vec2 vert_uv_out;

const int NUM_LIGHTS = 3;
uniform vec3 point_light_poses[NUM_LIGHTS];
uniform vec3 camera_pos;

vec3 phong(int point_light_pos_index)
{
   vec4 texel = texture(diffuse_tex, vert_uv_out);
   vec3 pixel_color = texel.xyz;
   vec3 point_light_pos = point_light_poses[point_light_pos_index];

   //Ambient
   float ambient_factor = 0.2f;
   vec3 ambient = pixel_color * ambient_factor;

   vec3 world_norm = texture(normal_tex, vert_uv_out).xyz;

   //Diffuse
   vec3 vert_world_pos_out = texture(position_tex, vert_uv_out).xyz;

   vec3 light_dir = normalize(point_light_pos - vert_world_pos_out);
   float lambert = max(dot(world_norm, light_dir), 0.f);
   vec3 diffuse = pixel_color * lambert;


   //Specular
   vec3 eye_dir = normalize(camera_pos - vert_world_pos_out);
   vec3 refl_dir = reflect(-light_dir, world_norm);
   float spec_str = max(dot(eye_dir, refl_dir), 0.f);
   float spec_fact = pow(spec_str, 40.f);
   vec3 specular = vec3(1.f, 1.f, 1.f) * spec_fact;

   //Phong
   vec3 phong = vec3(0, 0, 0);
   phong += ambient;
   phong += diffuse;
   phong += specular;


   //Table example: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
   float k_c = 1.f;
   float k_l = 0.045f; 
   float k_d = 0.0075f;

   float dist = length(point_light_pos - vert_world_pos_out);
   float attenuation = 1.f / ( k_c + k_l * dist + k_d * dist * dist);

   phong *= attenuation;

   return phong;
}

void main() 
{
   frag_color = vec4(0);

   for(int i = 0; i < NUM_LIGHTS; i++)
   {
      frag_color += vec4(phong(i), 1.f);
   }
}