#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_DiffuseRoughness;
uniform sampler2D ce_Depth;
uniform sampler2D ce_Normal;

in vec2 texCoord;


void main ()
{
    ce_FragColor.rgb = texture(ce_DiffuseRoughness, texCoord).rgb;
    ce_FragColor.a = 1.0;

}