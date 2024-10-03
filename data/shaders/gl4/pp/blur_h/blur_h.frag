#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color;
uniform float cs_TextureSizeInv;
uniform int cs_SampleCount;
uniform float cs_SampleScale;

in vec2 texCoord;

void main ()
{
    vec4 c = vec4(0, 0, 0, 0);
    float sum = 0.0;
    for (int i=-ce_SampleCount; i<=ce_SampleCount; i++)
    {
        vec2 coord = texCoord + vec2(float(i) * cs_TextureSizeInv * cs_SampleScale, 0.0);
        if (coord.x >= 0.0 && coord.x <= 1.0)
        {
            float f = abs(i) / float(cs_SampleCount);
            f = 1.0f -  f * f;
            sum += f;
            c += texture (cs_Color, coord) * f;
        }
    }
    c /= sum;
    cs_FragColor = c;
}

