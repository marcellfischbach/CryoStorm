

#version 330
layout (location = 0) out vec4 cs_FragColor;

uniform vec3 cs_Diffuse;
uniform float cs_Roughness;

in vec3 cs_vs_out_WorldPosition;
in vec3 cs_vs_out_WorldNormal;
in vec3 cs_vs_out_WorldTangent;

in vec3 cs_vs_out_CameraWorldPosition;
in vec3 cs_vs_out_CameraSpacePosition;
in vec2 cs_vs_out_ScreenCoordinates;
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




light_result_t calc_lights(vec3 frag_position, vec3 frag_normal, vec3 camera_space_position, vec3 viewer_position,float F0, float roughness)
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

     res.diffuse += lightRes.diffuse;
        res.specular += lightRes.
float calculate_ambient_lighting ()
{
    return 0.0;
}
//
// oren-nayar implementation of the diffuse lighting term
float calculate_diffuse_lighting (float n_dot_l, float n_dot_v, float roughness)
{
    float angleVN = acos(n_dot_v);
    float angleLN = acos(n_dot_l);

    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    float gamma = cos(angleVN - angleLN);

    float roughness2 = roughness * roughness;
    float A = 1.0 - 0.5 * roughness2 / (roughness2 + 0.33);
    float B = 0.45 *      roughness2 / (roughness2 + 0.09);
    float C = sin(alpha) * tan(beta);
    return clamp(n_dot_l * (A + (B * max(0.0, gamma) * C)), 0.0, 1.0);
}


//
// cook-torrance implementation of the specular lighting term
float calculate_specular_lighting(float F0, float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_v, float roughness)
{
    float Rs = 0.0;
    if (n_dot_l > 0)
    {

        // Fresnel reflectance
        float F = pow(1.0 - h_dot_v, 5.0);
        F *= (1.0 - F0);
        F += F0;

        // Microfacet distribution by Beckmann
        float m_squared = roughness * roughness;
        float r1 = 1.0 / (4.0 * m_squared * pow(n_dot_h, 4.0));
        float r2 = (n_dot_h * n_dot_h - 1.0) / (m_squared * n_dot_h * n_dot_h);
        float D = r1 * exp(r2);

        // Geometric shadowing
        float two_n_dot_h = 2.0 * n_dot_h;
        float g1 = (two_n_dot_h * n_dot_v) / h_dot_v;
        float g2 = (two_n_dot_h * n_dot_l) / h_dot_v;
        float G = min(1.0, min(g1, g2));

        Rs = (F * D * G) / (3.14159265 * n_dot_l * n_dot_v);
    }
    float k = 0.0;
    return clamp(n_dot_l * (k + Rs * (1.0 - k)), 0.0, 1.0);
}

uniform sampler2D cs_LightShadowMap[4];


float calculate_directional_shadow(int lightIdx, vec2 screen_coordinates, vec3 world_position, float distance_to_camera)
{
    return texture (cs_LightShadowMap[lightIdx], screen_coordinates).r;
}


float calculate_point_shadow(int lightIdx, vec2 screen_coordinates, vec3 world_position)
{
    return texture (cs_LightShadowMap[lightIdx], screen_coordinates).r;
}
void main ()
{
  float v_1 = 1.000000;

  vec3 norm = normalize (cs_vs_out_WorldNormal);

  vec3 tang = normalize (cs_vs_out_WorldTangent);
  vec3 binormal = normalize (cross(norm, tang));
  tang = cross(binormal, norm);

  mat3 normalMatrix = mat3(tang, binormal, norm);
  vec3 v_0 = vec3(0.500000, 0.500000, 1.000000);

  vec3 normal = v_0;
  normal = normal * 2.0 - 1.0;
  normal = normalMatrix * normal;

  float roughness = cs_Roughness;
  vec3 diffuse = cs_Diffuse;
  float alpha = v_1;
  float metallic = 0.000000;


  light_result_t light = calc_lights(cs_vs_out_WorldPosition, normal, cs_vs_out_CameraSpacePosition, cs_vs_out_CameraWorldPosition, 0.8, roughness);
  vec3 dielectric_light = light.diffuse  * diffuse + light.specular;
  vec3 metallic_light = light.specular * diffuse;

  cs_FragColor = vec4(mix(dielectric_light, metallic_light, metallic), alpha);

}
