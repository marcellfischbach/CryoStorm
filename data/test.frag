#version 330

layout(location = 0) out vec4 spc_FragColor;

uniform vec4 spc_Diffuse;

in vec4 color;

void main()
{
  spc_FragColor = color;
}

