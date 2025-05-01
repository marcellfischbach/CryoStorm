#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_UV) in vec2 cs_TexCoord;


out vec2 texCoord;

void main ()
{
    gl_Position = cs_Position;
    texCoord = cs_TexCoord;
}

