#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Color;
uniform float ce_HighValue;


in vec2 texCoord;

void main ()
{
    vec3 c = texture(ce_Color, texCoord).rgb;

    float l = length(c);
    if (c.r >= ce_HighValue || c.g >= ce_HighValue || c.b >= ce_HighValue)
    {
        float f = 1.0 - ce_HighValue;
        ce_FragColor = vec4(c - normalize(c) * ce_HighValue, 0.0) / f;
    }
    else
    {
        ce_FragColor = vec4(0, 0, 0, 0);
    }
}

