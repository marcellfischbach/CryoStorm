uniform int spc_LightCount;
uniform vec4 spc_LightColor[4];
uniform vec4 spc_LightVector[4];
uniform float spc_LightRange[4];
uniform int spc_LightCastShadow[4];


#include <directionallight_shadow.glsl>
#include <pointlight_shadow.glsl>
#include <oren-nayar.glsl>
#include <lambert.glsl>

struct lighting_result_t
{
    float ambient;
    float diffuse;
    float specular;
};

lighting_result_t calc_lighting (float n_dot_l, float n_dot_v, float roughness);

vec3 calc_light(int idx, vec3 light_color, vec4 light_vector, float light_range, vec3 frag_position, vec3  frag_normal, vec3 camera_space_position, float n_dot_v, float roughness)
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

        float n_dot_l = clamp(dot(frag_normal, frag_to_light), 0.0, 1.0);

        lighting_result_t lighting_result = calc_lighting(n_dot_l, n_dot_v, roughness);
        ambient = lighting_result.ambient;
        diffuse = lighting_result.diffuse;
        specular = lighting_result.specular;

        shadow = calc_point_shadow(idx, light_vector.xyz, light_range, frag_position);
        attenuation = max(1.0 - distance / light_range, 0.0);

    }
    else
    {
        float n_dot_l = clamp(dot(frag_normal, light_vector.xyz), 0.0, 1.0);

        lighting_result_t lighting_result = calc_lighting(n_dot_l, n_dot_v, roughness);
        ambient = lighting_result.ambient;
        diffuse = lighting_result.diffuse;
        specular = lighting_result.specular;
        shadow = calc_directional_shadow(idx, light_vector.xyz, frag_position, camera_space_position);
        attenuation = 1.0;
    }

    return light_color
    * (diffuse + specular + ambient)
    * shadow
    * attenuation;
}

vec3 calc_lights(vec3 frag_position, vec3 frag_normal, vec3 camera_space_position, vec3 viewer_position, float roughness)
{
    vec3 frag_to_viewer = normalize(viewer_position - frag_position);
    float n_dot_v = clamp(dot(frag_normal, frag_to_viewer), 0.0, 1.0);

    vec3 res = vec3(0, 0, 0);
    for (int i = 0; i < spc_LightCount; i++)
    {
        res += calc_light(i, spc_LightColor[i].rgb, spc_LightVector[i], spc_LightRange[i], frag_position, frag_normal, camera_space_position, n_dot_v, roughness);
    }
    return res;
}

