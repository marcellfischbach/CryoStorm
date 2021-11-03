#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;
uniform vec4 spc_Color;


in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;
in vec3 camera_space_position;
in vec3 viewer_world_position;

#include <../common/lighting.glsl>


void main()
{
    vec3 norm = normalize(world_normal);
    vec3 lightColor = calc_lights(world_position, norm, camera_space_position, viewer_world_position, 1.0);
    spc_FragColor = vec4(lightColor, 1.0) * spc_Color;
}

