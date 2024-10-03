#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color;
uniform vec2 cs_TextureSizeInv;


in vec2 texCoord;

void main ()
{
    cs_FragColor = (texture(cs_Color, texCoord)
    + texture(cs_Color, texCoord + vec2(cs_TextureSizeInv.x, 0))
    + texture(cs_Color, texCoord + vec2(0, cs_TextureSizeInv.y))
    + texture(cs_Color, texCoord + cs_TextureSizeInv)) / 4.0;
}

