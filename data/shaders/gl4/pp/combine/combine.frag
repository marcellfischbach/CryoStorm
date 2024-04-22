#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Color0;
uniform sampler2D ce_Color1;


in vec2 texCoord;

void main ()
{
    ce_FragColor = texture(ce_Color0, texCoord) + texture(ce_Color1, texCoord);
}

