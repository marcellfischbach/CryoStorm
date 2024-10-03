#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_Normals) in vec3 cs_Normal;
layout(location = eVS_Tangents) in vec3 cs_Tangent;
layout(location = eVS_Colors) in vec4 cs_Color;
layout(location = eVS_UV) in vec2 cs_UV;
layout(location = eVS_BoneIndices) in vec4 cs_BoneIndices;
layout(location = eVS_BoneWeights) in vec4 cs_BoneWeights;

uniform mat4 cs_ModelMatrix;
uniform mat4 cs_ModelViewProjectionMatrix;
uniform mat4 cs_SkeletonMatrices[32];


out vec2 texCoord;
out vec3 world_normal;
out vec3 world_tangent;

#include<../common/skinned.glsl>

void main()
{
  mat4 m0 = cs_SkeletonMatrices[int(cs_BoneIndices.x)];
  mat4 m1 = cs_SkeletonMatrices[int(cs_BoneIndices.y)];
  mat4 m2 = cs_SkeletonMatrices[int(cs_BoneIndices.z)];
  mat4 m3 = cs_SkeletonMatrices[int(cs_BoneIndices.w)];


  vec4 position = skinned_calc_position(cs_Position, m0, m1, m2, m3, cs_BoneWeights);
  vec3 normal = skinned_calc_normal(cs_Normal, m0, m1, m2, m3, cs_BoneWeights);
  vec3 tangent = skinned_calc_normal(cs_Tangent, m0, m1, m2, m3, cs_BoneWeights);

  world_normal = (cs_ModelMatrix * vec4(normal, 0.0)).xyz;
  world_tangent = (cs_ModelMatrix * vec4(tangent, 0.0)).xyz;

  gl_Position = cs_ModelViewProjectionMatrix * position;
  texCoord = cs_UV;
}

