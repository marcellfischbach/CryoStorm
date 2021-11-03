uniform int spc_LightCount;
uniform vec4 spc_LightColor[4];
uniform vec4 spc_LightVector[4];
uniform float spc_LightRange[4];
uniform int spc_LightCastShadow[4];


#include <directionallight_shadow.glsl>
#include <pointlight_shadow.glsl>
#include <oren-nayar.glsl>


vec3 calc_light(int idx, vec3 light_color, vec4 light_vector, float light_range, vec3 frag_position, vec3  frag_normal, vec3 camera_space_position, vec3 viewer_position, float roughness)
{
    vec3 frag_to_viewer = normalize(viewer_position - frag_position);

    if (light_vector.w == 1.0)
    {
        vec3 frag_to_light = light_vector.xyz - frag_position;
        float distance = length(frag_to_light);
        frag_to_light /= distance;

//        return calc_point_light(light_color, light_vector.xyz, light_range, frag_position, frag_normal)
        return calc_lighting (light_color, frag_to_light, frag_normal, frag_to_viewer, roughness)
        * calc_point_shadow(idx, light_vector.xyz, light_range, frag_position)
        * max(1.0 - distance / light_range, 0.0);
    }
    else
    {

        //        return calc_directional_light(light_color, light_vector.xyz, frag_normal, normalize(viewer_position - frag_position))
        return calc_lighting (light_color, light_vector.xyz, frag_normal, frag_to_viewer, roughness)
            * calc_directional_shadow(idx, light_vector.xyz, frag_position, camera_space_position);
    }
}

vec3 calc_lights(vec3 frag_position, vec3 frag_normal, vec3 camera_space_position, vec3 viewer_position, float roughness)
{
    vec3 res = vec3(0, 0, 0);
    for (int i = 0; i < spc_LightCount; i++)
    {
        res += calc_light(i, spc_LightColor[i].rgb, spc_LightVector[i], spc_LightRange[i], frag_position, frag_normal, camera_space_position, viewer_position, roughness);
    }
    return res;
}

