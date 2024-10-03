#version 330

layout(location = eVS_Vertices) in vec3 cs_Position;
layout(location = eVS_UV) in vec2 cs_TexCoord;


out vec2 texCoord;

void main ()
{
    gl_Position = vec4(cs_Position, 1.0);
    texCoord = cs_TexCoord;
}

