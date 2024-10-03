uniform int cs_LightCount;
uniform vec4 cs_LightAmbient[4];
uniform vec4 cs_LightColor[4];
uniform vec4 cs_LightVector[4];
uniform float cs_LightRange[4];
uniform int cs_LightCastShadow[4];

uniform int cs_ReceiveShadow;

float calculate_ambient_lighting ();
float calculate_diffuse_lighting (float n_dot_l, float n_dot_v, float roughness);
float calculate_specular_lighting(float F0, float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_v, float roughness);

float calculate_directional_shadow(int lightIdx, vec2 screen_coordinates, vec3 world_position, float distance_to_camera);
float calculate_point_shadow(int lightIdx, vec2 screen_coordinates, vec3 world_position);



struct light_result_t
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

light_result_t calc_light(int idx, vec3 light_ambient, vec3 light_color, vec4 light_vector, float light_range, vec3 frag_position, vec3  frag_normal, vec3 camera_space_position, float n_dot_v, vec3 frag_to_viewer, float F0, float roughness)
{
    float diffuse = 1.0;
    float specular = 0.0;
    float ambient = 0.0;
    float shadow = 1.0;
    float attenuation = 1.0;

    if (light_vector.w == 1.0)
    {
        vec3 frag_to_light = light_vector.xyz - frag_position;

        float distance = length(frag_to_light);
        frag_to_light /= distance;

        vec3 H = normalize(frag_to_light + frag_to_viewer);
        float n_dot_l = clamp(dot(frag_normal, frag_to_light), 0.0, 1.0);
        float n_dot_h = clamp(dot(frag_normal, H), 0.0, 1.0);
        float h_dot_v = clamp(dot(H, frag_to_viewer), 0.0, 1.0);

        ambient = calculate_ambient_lighting();
        diffuse = calculate_diffuse_lighting (n_dot_l, n_dot_v, roughness);
        specular = calculate_specular_lighting(F0, n_dot_l, n_dot_v, n_dot_h, h_dot_v, roughness);

        attenuation = clamp(1.0 - distance / light_range, 0.0, 1.0);

        if (cs_LightCastShadow[idx] > 0 && cs_ReceiveShadow > 0)
        {
            shadow = calculate_point_shadow(idx, cs_vs_out_ScreenCoordinates, frag_position);
        }
    }
    else
    {
        vec3 H = normalize(light_vector.xyz + frag_to_viewer);
        float n_dot_l = clamp(dot(frag_normal, light_vector.xyz), 0.0, 1.0);
        float n_dot_h = clamp(dot(frag_normal, H), 0.0, 1.0);
        float h_dot_v = clamp(dot(H, frag_to_viewer), 0.0, 1.0);

        ambient = calculate_ambient_lighting();
        diffuse = calculate_diffuse_lighting (n_dot_l, n_dot_v, roughness);
        specular = calculate_specular_lighting(F0, n_dot_l, n_dot_v, n_dot_h, h_dot_v, roughness);
        attenuation = 1.0;

        if (cs_LightCastShadow[idx] > 0 && cs_ReceiveShadow > 0)
        {
            shadow = calculate_directional_shadow(idx, cs_vs_out_ScreenCoordinates, frag_position, camera_space_position.z);
        }
    }



    diffuse = clamp(diffuse, 0.0, 1.0);
    specular = clamp(specular, 0.0, 1.0);
    ambient = clamp(ambient, 0.0, 1.0);


    float attShadow = shadow * attenuation;
    light_result_t res;
    res.ambient = ambient * cs_LightAmbient[idx].rgb;
    res.diffuse = light_color * diffuse * attShadow;
    res.specular = light_color * specular * attShadow;

    return res;
}




light_result_t calc_lights(vec3 frag_position, vec3 frag_normal, vec3 camera_space_position, vec3 viewer_position, float F0, float roughness)
{
    vec3 frag_to_viewer = normalize(viewer_position - frag_position);
    float n_dot_v = clamp(dot(frag_normal, frag_to_viewer), 0.0, 1.0);

    light_result_t res;
    res.ambient = vec3(0, 0, 0);
    res.diffuse = vec3(0, 0, 0);
    res.specular = vec3(0, 0, 0);
    for (int i = 0; i < cs_LightCount; i++)
    {
        light_result_t lightRes = calc_light(i, cs_LightAmbient[i].rgb, cs_LightColor[i].rgb, cs_LightVector[i], cs_LightRange[i], frag_position, frag_normal, camera_space_position, n_dot_v, frag_to_viewer, F0, roughness);
        res.ambient += lightRes.ambient;
        res.diffuse += lightRes.diffuse;
        res.specular += lightRes.specular;
    }
    return res;
}

