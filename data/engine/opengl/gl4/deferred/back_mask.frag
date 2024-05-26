#version 330
layout(location = 0) out vec4 ce_FragColor;

uniform sampler2D ce_Depth;

in vec2 texCoord;

void main ()
{
    float d = texture(ce_Depth, texCoord).r;
    if (d == 1.0)
    {
        discard;
    }

    ce_FragColor = vec4(0, 0, 0, 0);
}