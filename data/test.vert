#version 330

layout(location = eVS_Vertices) in vec4 spc_Position;
layout(location = eVS_Colors) in vec4 spc_Color;

out vec4 color;

void main()
{
  gl_Position = spc_Position;
  color = spc_Color;
}