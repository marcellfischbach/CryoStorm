uniform sampler2D ce_LightShadowMap[4];


float calculate_directional_shadow(int lightIdx, vec2 screen_coordinates, vec3 world_position, float distance_to_camera)
{
    return texture (ce_LightShadowMap[lightIdx], screen_coordinates).r;
}


float calculate_point_shadow(int lightIdx, vec2 screen_coordinates, vec3 world_position)
{
    return texture (ce_LightShadowMap[lightIdx], screen_coordinates).r;
}