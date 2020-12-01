#version 330

layout(location = eVS_Vertices) in vec4 spc_Position;
layout(location = eVS_Colors) in vec4 spc_Color;
layout(location = eVS_UV) in vec2 spc_UV;


uniform mat4 spc_ModelViewMatrix;
uniform mat4 spc_ProjectionMatrix;

out vec4 color;
out vec2 texCoord;

void main()
{
  gl_Position = spc_ProjectionMatrix * spc_ModelViewMatrix * spc_Position;
  color = spc_Color;
  texCoord = spc_UV;
}