#version 330

layout(location = 0) in vec4 spc_Position;

void main()
{
  gl_Position = spc_Position;
}