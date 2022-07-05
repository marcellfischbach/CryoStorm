#version 330

layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Diffuse;
uniform vec4 ce_Color;
uniform float ce_Roughness;


in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;
in vec3 camera_space_position;
in vec3 viewer_world_position;

#include <../common/lighting.glsl>

lighting_result_t calc_lighting (float n_dot_l, float n_dot_v, float n_dot_h, float h_dot_l, float h_dot_v, float roughness)
{
    lighting_result_t res;
    res.ambient = 0.0;
    res.diffuse = oren_nayar (n_dot_l, n_dot_v, roughness);
    res.specular = cook_torrance(0.8, n_dot_l, n_dot_v, n_dot_h, h_dot_v, roughness);
    return res;
}


void main()
{
    vec3 norm = normalize(world_normal);
    vec3 frag_light = calc_lights(world_position, norm, ce_Roughness, camera_space_position, viewer_world_position);
    vec4 texColor = texture(ce_Diffuse, texCoord * 3);
      ce_FragColor = vec4(frag_light, 1.0) * ce_Color * texColor;
//    ce_FragColor = vec4(frag_light, 1.0) * ce_Color;// * texColor;
//    ce_FragColor = vec4(norm * 0.5 + 0.5, 1.0) * texColor;
}

