#version 330

layout(location = eVS_Vertices) in vec4 cs_Position;
layout(location = eVS_BoneIndices) in vec4 cs_BoneIndices;
layout(location = eVS_BoneWeights) in vec4 cs_BoneWeights;


uniform mat4 cs_ModelViewProjectionMatrix;
uniform mat4 cs_SkeletonMatrices[32];


#include<../common/skinned.glsl>

void main()
{
    mat4 m0 = cs_SkeletonMatrices[int(cs_BoneIndices.x)];
    mat4 m1 = cs_SkeletonMatrices[int(cs_BoneIndices.y)];
    mat4 m2 = cs_SkeletonMatrices[int(cs_BoneIndices.z)];
    mat4 m3 = cs_SkeletonMatrices[int(cs_BoneIndices.w)];


    vec4 position = skinned_calc_position(cs_Position, m0, m1, m2, m3, cs_BoneWeights);

    gl_Position = cs_ModelViewProjectionMatrix * position;
//    texCoord = cs_UV;
 }

