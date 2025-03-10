#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Depth;

in vec2 texCoord;

void main ()
{
    float d = texture(cs_Depth, texCoord).r;
    if (d == 1.0)
    {
        discard;
    }

    cs_FragColor = vec4(0, 0, 0, 0);
}