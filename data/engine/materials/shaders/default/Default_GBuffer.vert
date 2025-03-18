#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_Normals) in vec3 cs_Normal;
layout(location = eVS_Tangents) in vec3 cs_Tangent;
layout(location = eVS_Colors) in vec4 cs_Color;
layout(location = eVS_UV) in vec2 cs_UV;


uniform mat4 cs_ModelMatrix;
uniform mat4 cs_ModelViewProjectionMatrix;


out vec2 texCoord;
out vec3 world_normal;
out vec3 world_tangent;

void main()
{

  world_normal = (cs_ModelMatrix * vec4(cs_Normal, 0.0)).xyz;
  world_tangent = (cs_ModelMatrix * vec4(cs_Tangent, 0.0)).xyz;

  gl_Position = cs_ModelViewProjectionMatrix * cs_Position;
  texCoord = cs_UV;
}

