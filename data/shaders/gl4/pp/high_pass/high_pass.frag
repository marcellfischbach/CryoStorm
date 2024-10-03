#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color;
uniform float cs_HighValue;


in vec2 texCoord;

void main ()
{
    vec3 c = texture(cs_Color, texCoord).rgb;

    float l = length(c);
    if (c.r >= cs_HighValue || c.g >= cs_HighValue || c.b >= cs_HighValue)
    {
        float f = 1.0 - cs_HighValue;
        cs_FragColor = vec4(c - normalize(c) * cs_HighValue, 0.0) / f;
    }
    else
    {
        cs_FragColor = vec4(0, 0, 0, 0);
    }
}

