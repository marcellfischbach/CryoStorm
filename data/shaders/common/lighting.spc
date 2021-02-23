fragment "

#include<lighting_pointlight.spc>

vec4 calc_directional_light(vec4 light_color, vec3 light_vector, vec3 frag_normal)
{
  return light_color * clamp(dot(light_vector, frag_normal), 0.0, 1.0);
}

vec4 calc_light(vec4 light_color, vec4 light_vector, float light_range, vec3 frag_position, vec3  frag_normal)
{
    if (light_vector.w == 1.0)
    {
        return calc_point_light(light_color, light_vector.xyz, light_range, frag_position, frag_normal);
    }
    else
    {
        return calc_directional_light(light_color, light_vector.xyz, frag_normal);
    }
}
"