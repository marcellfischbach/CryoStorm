#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Color;
uniform vec2 ce_TextureSizeInv;


in vec2 texCoord;

void main ()
{
    ce_FragColor = (texture(ce_Color, texCoord)
    + texture(ce_Color, texCoord + vec2(ce_TextureSizeInv.x, 0))
    + texture(ce_Color, texCoord + vec2(0, ce_TextureSizeInv.y))
    + texture(ce_Color, texCoord + ce_TextureSizeInv)) / 4.0;
}

