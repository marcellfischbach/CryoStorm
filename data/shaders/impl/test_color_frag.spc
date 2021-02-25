
shader "fragment" "
#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform sampler2D spc_Diffuse;
uniform vec4 spc_Color;

uniform vec4 spc_LightColor[4];
uniform vec4 spc_LightVector[4];
uniform float spc_LightRange[4];
uniform int spc_LightCount;

in vec4 color;
in vec2 texCoord;
in vec3 world_position;
in vec3 world_normal;

#include <../common/lighting.spc>

void main()
{
    vec3 norm = normalize(world_normal);
    vec4 frag_color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < spc_LightCount; i++)
    {
      frag_color += calc_light(spc_LightColor[i], spc_LightVector[i], spc_LightRange[i], world_position, norm);
    }
    frag_color *= color;
    frag_color *= texture(spc_Diffuse, texCoord);
    spc_FragColor = frag_color;
    //  spc_FragColor = vec4(norm * 0.5 + 0.5, 1.0);
    spc_FragColor = spc_Color;
}

"