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
        float f = 1.0f - abs(i) / float(ce_SampleCount);
        sum += f;
        c += texture (ce_Color, texCoord + vec2(0.0f, float(i) * ce_TextureSizeInv * ce_SampleScale)) * f;
    }
    c /= sum;
    ce_FragColor = c;
}

