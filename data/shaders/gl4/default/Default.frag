#version 330

layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Diffuse;
uniform vec4 ce_Color;
uniform float ce_Roughness;
uniform float ce_Metallic;


in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;
in vec3 camera_space_position;
in vec3 viewer_world_position;

#include <../common/lighting.glsl>

lighting_result_t calc_lighting (float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_l, float h_dot_v)
{
    lighting_result_t res;
    res.ambient = 0.0;
    res.diffuse = oren_nayar (n_dot_l, n_dot_v, ce_Roughness);
    res.specular = cook_torrance(0.8, n_dot_l, n_dot_v, n_dot_h, h_dot_v, ce_Roughness);
    return res;
}


void main()
{
    vec3 norm = normalize(world_normal);
    light_result_t light = calc_lights(world_position, norm, camera_space_position, viewer_world_position);
    vec4 color = texture(ce_Diffuse, texCoord * 3) * ce_Color;


    vec3 dielectric = light.diffuse  * color.rgb + light.specular;
    vec3 metallic = light.specular * color.rgb;


    ce_FragColor = vec4(mix(dielectric, metallic, ce_Metallic), color.a);
}



