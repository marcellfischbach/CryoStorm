#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_Colors) in vec4 ce_Color;
layout(location = eVS_UV) in vec2 ce_UV;


uniform mat4 ce_ModelViewProjectionMatrix;

out vec4 color;
out vec2 texCoord;

void main()
{
  gl_Position = ce_ModelViewProjectionMatrix * ce_Position;
  color = ce_Color;
  texCoord = ce_UV;
}

