#version 330

layout(location = eVS_Vertices) in vec4 ce_Position;
layout(location = eVS_UV) in vec2 ce_UV;
layout(location = eVS_BoneIndices) in vec4 ce_BoneIndices;
layout(location = eVS_BoneWeights) in vec4 ce_BoneWeights;


uniform mat4 ce_ModelViewProjectionMatrix;
uniform mat4 ce_SkeletonMatrices[32];


out vec2 texCoord;


#include<../common/skinned.glsl>

void main()
{
	mat4 m0 = ce_SkeletonMatrices[int(ce_BoneIndices.x)];
	mat4 m1 = ce_SkeletonMatrices[int(ce_BoneIndices.y)];
	mat4 m2 = ce_SkeletonMatrices[int(ce_BoneIndices.z)];
	mat4 m3 = ce_SkeletonMatrices[int(ce_BoneIndices.w)];


	vec4 position = skinned_calc_position(ce_Position, m0, m1, m2, m3, ce_BoneWeights);
	gl_Position = ce_ModelViewProjectionMatrix * position;
	texCoord = ce_UV;
}

