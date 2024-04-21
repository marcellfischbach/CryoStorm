#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Color;
uniform float ce_HighValue;


in vec2 texCoord;

void main ()
{
    vec4 c = texture(ce_Color, texCoord);

    if (c.r >= ce_HighValue || c.g >= ce_HighValue || c.b >= ce_HighValue)
    {
        ce_FragColor = c;
    }
    else
    {
        ce_FragColor = vec4(0, 0, 0, 0);
    }
}

