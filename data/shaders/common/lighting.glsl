uniform int ce_LightCount;
uniform vec4 ce_LightColor[4];
uniform vec4 ce_LightVector[4];
uniform float ce_LightRange[4];
uniform int ce_LightCastShadow[4];


#include <directionallight_shadow.glsl>
#include <pointlight_shadow.glsl>
#include <oren-nayar.glsl>
#include <lambert.glsl>
#include <cook-torrance.glsl>

struct lighting_result_t
{
    float ambient;
    float diffuse;
    float specular;
};

lighting_result_t calc_lighting (float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_l, float h_dot_v);

vec3 calc_light(int idx, vec3 light_color, vec4 light_vector, float light_range, vec3 frag_position, vec3  frag_normal, vec3 camera_space_position, float n_dot_v, vec3 frag_to_viewer)
{
    float diffuse = 1.0;
    float specular = 0.0;
    float ambient = 0.0;
    float shadow = 1.0;
    float attenuation = 1.0;

    if (light_vector.w == 1.0)
    {
        vec3 frag_to_light = light_vector.xyz - frag_position;
        vec3 H = normalize(frag_to_light + frag_to_viewer);

        float distance = length(frag_to_light);
        frag_to_light /= distance;

        float n_dot_l = clamp(dot(frag_normal, frag_to_light), 0.0, 1.0);
        float n_dot_h = clamp(dot(frag_normal, H), 0.0, 1.0);
        float h_dot_l = clamp(dot(H, frag_to_light), 0.0, 1.0);
        float h_dot_v = clamp(dot(H, frag_to_viewer), 0.0, 1.0);

        lighting_result_t lighting_result = calc_lighting(n_dot_l, n_dot_v, n_dot_h, h_dot_l, h_dot_v);
        ambient = lighting_result.ambient;
        diffuse = lighting_result.diffuse;
        specular = lighting_result.specular;

        shadow = calc_point_shadow(idx, light_vector.xyz, light_range, frag_position);
        attenuation = max(1.0 - distance / light_range, 0.0);

    }
    else
    {
        vec3 H = normalize(light_vector.xyz + frag_to_viewer);
        float n_dot_l = clamp(dot(frag_normal, light_vector.xyz), 0.0, 1.0);
        float n_dot_h = clamp(dot(frag_normal, H), 0.0, 1.0);
        float h_dot_l = clamp(dot(H, light_vector.xyz), 0.0, 1.0);
        float h_dot_v = clamp(dot(H, frag_to_viewer), 0.0, 1.0);

        lighting_result_t lighting_result = calc_lighting(n_dot_l, n_dot_v, n_dot_h, h_dot_l, h_dot_v);
        ambient = lighting_result.ambient;
        diffuse = lighting_result.diffuse;
        specular = lighting_result.specular;
        shadow = calc_directional_shadow(idx, light_vector.xyz, frag_position, camera_space_position);
        attenuation = 1.0;
    }

    diffuse = clamp(diffuse, 0.0, 1.0);
    specular = clamp(specular, 0.0, 1.0);
    ambient = clamp(ambient, 0.0, 1.0);
    return light_color
    * clamp(diffuse + specular + ambient, 0.0, 1.0)
    * shadow
    * attenuation;
}

vec3 calc_lights(vec3 frag_position, vec3 frag_normal, vec3 camera_space_position, vec3 viewer_position)
{
    vec3 frag_to_viewer = normalize(viewer_position - frag_position);
    float n_dot_v = clamp(dot(frag_normal, frag_to_viewer), 0.0, 1.0);

    vec3 res = vec3(0, 0, 0);
    for (int i = 0; i < ce_LightCount; i++)
    {
        res += calc_light(i, ce_LightColor[i].rgb, ce_LightVector[i], ce_LightRange[i], frag_position, frag_normal, camera_space_position, n_dot_v, frag_to_viewer);
    }
    return res;
}

