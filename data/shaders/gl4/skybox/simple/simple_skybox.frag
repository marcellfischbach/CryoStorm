#version 330
layout (location = 0)  out vec4 ce_FragColor;


in vec4 color;
in vec3 uv;

void main ()
{
    ce_FragColor = color;
}