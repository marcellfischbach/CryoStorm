#version 330
layout(location = 0) out vec4 cs_FragColor;

uniform sampler2DArray cs_Diffuse;
uniform float cs_ArrayIndex;


in vec2 texCoord;

void main ()
{
    float r = texture(cs_Diffuse, vec3(texCoord, 1.0)).r;
    cs_FragColor = vec4(r, r, r, 1.0);
}

