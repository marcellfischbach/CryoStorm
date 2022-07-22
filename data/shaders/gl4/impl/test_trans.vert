#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_Normals) in vec3 ce_Normal;
layout(location = eVS_Colors) in vec4 ce_Color;
layout(location = eVS_UV) in vec2 ce_UV;


uniform mat4 ce_ModelMatrix;
uniform mat4 ce_ViewMatrix;
uniform mat4 ce_ViewProjectionMatrix;

out vec4 color;
out vec2 texCoord;
out vec3 world_position;
out vec3 world_normal;
out vec3 camera_space_position;
out vec3 viewer_world_position;

void main()
{
  vec4 position = ce_ModelMatrix * ce_Position;
  world_position = position.xyz;
  world_normal = (ce_ModelMatrix * vec4(ce_Normal, 0.0)).xyz;

  viewer_world_position = -(ce_ViewMatrix * vec4(0, 0, 0, 1)).xyz;
  camera_space_position = (ce_ViewMatrix * position).xyz;

  gl_Position = ce_ViewProjectionMatrix * position;
  color = ce_Color;
  texCoord = ce_UV;
}

