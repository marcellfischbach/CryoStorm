#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Color;
uniform float ce_TextureSizeInv;
uniform int ce_SampleCount;
uniform float ce_SampleScale;

in vec2 texCoord;

void main ()
{
    vec4 c = vec4(0, 0, 0, 0);
    float sum = 0.0;
    for (int i=-ce_SampleCount; i<=ce_SampleCount; i++)
    {
        vec2 coord = texCoord + vec2(float(i) * ce_TextureSizeInv * ce_SampleScale, 0.0);
        if (coord.x >= 0.0 && coord.x <= 1.0)
        {
            float f = abs(i) / float(ce_SampleCount);
            f = 1.0f -  f * f;
            sum += f;
            c += texture (ce_Color, coord) * f;
        }
    }
    c /= sum;
    ce_FragColor = c;
}

