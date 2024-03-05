#pragma once

#include <assimp/color4.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/mesh.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>


namespace ce
{
struct Color4f;
struct iRenderMesh;
struct Matrix4f;
struct Vector2f;
struct Vector3f;
class Skeleton;
}



namespace ce::assimp
{

Color4f ConvertRGBA(aiColor4D &v);
Vector2f Convert2f(aiVector3D &v);
Vector3f Convert3f(aiVector3D &v);
Matrix4f ConvertMatrix4x4(aiMatrix4x4 &aiMat);

iRenderMesh *ConvertRenderMesh(aiMesh *mesh, const Matrix4f &matrix,  Skeleton* skeleton);

}