#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_UV) in vec2 cs_UV;



out vec2 gs_uv;

void main ()
{
    gl_Position = cs_Position;
    gs_uv = cs_Position.xy;
}