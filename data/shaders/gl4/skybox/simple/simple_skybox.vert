#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_UV) in vec2 ce_UV;



out vec2 gs_uv;

void main ()
{
    gl_Position = ce_Position;
    gs_uv = ce_Position.xy;
}