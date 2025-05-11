#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_UV) in vec2 cs_TexCoord;

uniform vec4 cs_PartialRect;

out vec2 texCoord;

void main ()
{
    gl_Position = vec4(
    cs_PartialRect.x + cs_Position.x * cs_PartialRect.z,
    cs_PartialRect.y + cs_Position.y * cs_PartialRect.w,
    cs_Position.z,
    1.0);
    texCoord = cs_TexCoord;
}

