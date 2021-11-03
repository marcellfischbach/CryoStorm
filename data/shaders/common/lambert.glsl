

vec3 calc_lighting (vec3 light_color, vec3 frag_to_light, vec3 frag_normal, vec3 frag_to_viewer, float roughness)
{
    return light_color * clamp(dot(frag_to_light, frag_normal), 0.0, 1.0);
}
