#pragma once



namespace ce
{
struct Color4f;
struct iRenderMesh;
struct Matrix4f;
struct Vector2f;
struct Vector3f;
class Skeleton;
}


template <typename TReal> class aiColor4t;
template <typename TReal> class aiVector3t;
template <typename TReal> class aiMatrix4x4t;
typedef aiColor4t<float> aiColor4D;
typedef aiVector3t<float> aiVector3D;
typedef aiMatrix4x4t<float> aiMatrix4x4;

struct aiMesh;


namespace ce::assimp
{

Color4f ConvertRGBA(aiColor4D &v);
Vector2f Convert2f(aiVector3D &v);
Vector3f Convert3f(aiVector3D &v);
Matrix4f ConvertMatrix4x4(aiMatrix4x4 &aiMat);

iRenderMesh *ConvertRenderMesh(aiMesh *mesh, const Matrix4f &matrix,  Skeleton* skeleton);

}