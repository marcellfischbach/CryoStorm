#version 330

layout(location = eVS_Vertices) in vec4 spc_Position;
layout(location = eVS_Normals) in vec3 spc_Normal;
layout(location = eVS_Colors) in vec4 spc_Color;
layout(location = eVS_UV) in vec2 spc_UV;


uniform mat4 spc_ModelMatrix;
uniform mat4 spc_ViewProjectionMatrix;

out vec4 color;
out vec2 texCoord;
out vec3 world_position;
out vec3 world_normal;

void main()
{
  vec4 position = spc_ModelMatrix * spc_Position;
  world_position = position.xyz;
  world_normal = (spc_ModelMatrix * vec4(spc_Normal, 0.0)).xyz;

  gl_Position = spc_ViewProjectionMatrix * position;
  color = spc_Color;
  texCoord = spc_UV;
}

