#version 330

layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Diffuse;
uniform vec4 ce_Color;


in vec4 color;
in vec2 texCoord;


void main()
{
    vec4 texColor = texture(ce_Diffuse, texCoord * 3);
    ce_FragColor = ce_Color * color * texColor;
}

