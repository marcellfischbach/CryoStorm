#version 330

layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Diffuse;
uniform vec4 ce_Color;


in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;
in vec3 camera_space_position;
in vec3 viewer_world_position;

#include <../common/lighting.glsl>

lighting_result_t calc_lighting (float n_dot_l, float n_dot_v)
{
    lighting_result_t res;
    res.ambient = 0.0;
    res.diffuse = oren_nayar (n_dot_l, n_dot_v, 0.0);
    res.specular = 0.0;
    return res;
}

void main()
{
    vec3 norm = normalize(world_normal);
    vec3 lightColor = calc_lights(world_position, norm, camera_space_position, viewer_world_position);
    ce_FragColor = vec4(lightColor, 1.0) * ce_Color;
}

