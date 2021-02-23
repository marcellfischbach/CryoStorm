fragment "

vec4 calc_point_light(vec4 light_color, vec3 light_position, float light_range, vec3 frag_position, vec3  frag_normal)
{
    vec3 v_to_l = light_position - frag_position;
    float distance = length(v_to_l);

  v_to_l /= distance;
  return light_color
            * clamp(dot(v_to_l, frag_normal), 0.0, 1.0)
            * max(1.0 - distance / light_range, 0.0);
}
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