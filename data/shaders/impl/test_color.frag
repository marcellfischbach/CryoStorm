#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;
uniform vec4 spc_Color;
uniform float spc_Roughness;


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
    vec3 frag_light = calc_lights(world_position, norm, camera_space_position, viewer_world_position, spc_Roughness);
    vec4 texColor = texture(spc_Diffuse, texCoord * 3);
    texColor = vec4(1, 1, 1, 1);
    spc_FragColor = vec4(frag_light, 1.0) * spc_Color * texColor;
}

