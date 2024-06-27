uniform int ce_LightCount;
uniform vec4 ce_LightAmbient[4];
uniform vec4 ce_LightColor[4];
uniform vec4 ce_LightVector[4];
uniform float ce_LightRange[4];
uniform int ce_LightCastShadow[4];
uniform sampler2D ce_LightShadowMap[4];

uniform int ce_ReceiveShadow;

float calculate_ambient_lighting ();
float calculate_diffuse_lighting (float n_dot_l, float n_dot_v, float roughness);
float calculate_specular_lighting(float F0, float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_v, float roughness);



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
        diffuse = calculate_diffuse_lightinga (n_dot_l, n_dot_v, roughness);
        specular = calculate_specular_lighting(F0, n_dot_l, n_dot_v, n_dot_h, h_dot_v, roughness);

        attenuation = clamp(1.0 - distance / light_range, 0.0, 1.0);

    }
    else
    {
        vec3 H = normalize(light_vector.xyz + frag_to_viewer);
        float n_dot_l = clamp(dot(frag_normal, light_vector.xyz), 0.0, 1.0);
        float n_dot_h = clamp(dot(frag_normal, H), 0.0, 1.0);
        float h_dot_v = clamp(dot(H, frag_to_viewer), 0.0, 1.0);

        ambient = calculate_ambient_lighting();
        diffuse = calculate_diffuse_lightinga (n_dot_l, n_dot_v, roughness);
        specular = calculate_specular_lighting(F0, n_dot_l, n_dot_v, n_dot_h, h_dot_v, roughness);
        attenuation = 1.0;
    }

    if (ce_LightCastShadow[idx] > 0 && ce_ReceiveShadow > 0)
    {
        shadow = texture (ce_LightShadowMap[idx], screen_coord).r;
    }

    diffuse = clamp(diffuse, 0.0, 1.0);
    specular = clamp(specular, 0.0, 1.0);
    ambient = clamp(ambient, 0.0, 1.0);


    float attShadow = shadow * attenuation;
    light_result_t res;
    res.ambient = ambient * ce_LightAmbient[idx].rgb;
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
    for (int i = 0; i < ce_LightCount; i++)
    {
        light_result_t lightRes = calc_light(i, ce_LightAmbient[i].rgb, ce_LightColor[i].rgb, ce_LightVector[i], ce_LightRange[i], frag_position, frag_normal, camera_space_position, n_dot_v, frag_to_viewer, F0, roughness);
        res.ambient += lightRes.ambient;
        res.diffuse += lightRes.diffuse;
        res.specular += lightRes.specular;
    }
    return res;
}

