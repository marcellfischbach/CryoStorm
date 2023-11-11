#version 330

layout(location = eVS_Vertices) in vec3 ce_Position;
layout(location = eVS_UV) in vec2 ce_TexCoord;


out vec2 texCoord;
out vec2 ndc;

void main ()
{
    gl_Position = vec4(ce_Position, 1.0);
    texCoord = ce_TexCoord;
    ndc = ce_TexCoord * 2.0 - 1.0;

}

