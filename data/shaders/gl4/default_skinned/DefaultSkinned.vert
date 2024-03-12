#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_Normals) in vec3 ce_Normal;
layout(location = eVS_Tangents) in vec3 ce_Tangent;
layout(location = eVS_Colors) in vec4 ce_Color;
layout(location = eVS_UV) in vec2 ce_UV;
layout(location = eVS_BoneIndices) in vec4 ce_BoneIndices;
layout(location = eVS_BoneWeights) in vec4 ce_BoneWeights;


uniform mat4 ce_ModelMatrix;
uniform mat4 ce_ViewMatrix;
uniform mat4 ce_ViewMatrixInv;
uniform mat4 ce_ViewProjectionMatrix;
uniform mat4 ce_ModelViewProjectionMatrix;
uniform mat4 ce_SkeletonMatrices[32];

out vec4 color;
out vec2 texCoord;
out vec3 world_position;
out vec3 world_normal;
out vec3 world_tangent;
out vec3 camera_space_position;
out vec3 viewer_world_position;
out vec2 screen_coord;

#include<../common/skinned.glsl>

void main()
{
  mat4 m0 = ce_SkeletonMatrices[int(ce_BoneIndices.x)];
  mat4 m1 = ce_SkeletonMatrices[int(ce_BoneIndices.y)];
  mat4 m2 = ce_SkeletonMatrices[int(ce_BoneIndices.z)];
  mat4 m3 = ce_SkeletonMatrices[int(ce_BoneIndices.w)];


  vec4 position = skinned_calc_position(ce_Position, m0, m1, m2, m3, ce_BoneWeights);
  vec3 normal = skinned_calc_normal(ce_Normal, m0, m1, m2, m3, ce_BoneWeights);
  vec3 tangent = skinned_calc_normal(ce_Tangent, m0, m1, m2, m3, ce_BoneWeights);

  world_position = (ce_ModelMatrix * position).xyz;
  world_normal = (ce_ModelMatrix * vec4(normal, 0.0)).xyz;
  world_tangent = (ce_ModelMatrix * vec4(tangent, 0.0)).xyz;

  viewer_world_position = (ce_ViewMatrixInv * vec4(0, 0, 0, 1)).xyz;
  camera_space_position = (ce_ViewMatrix * position).xyz;

  gl_Position = ce_ModelViewProjectionMatrix * position;
  color = ce_Color;
  texCoord = ce_UV;
  screen_coord = (gl_Position.xy / gl_Position.w) * 0.5 + 0.5;
}

