#pragma once



namespace cs
{
struct csColor4f;
struct iRenderMesh;
struct csMatrix4f;
struct csVector2f;
struct csVector3f;
class csSkeleton;
}


template <typename TReal> class aiColor4t;
template <typename TReal> class aiVector3t;
template <typename TReal> class aiMatrix4x4t;
typedef aiColor4t<float> aiColor4D;
typedef aiVector3t<float> aiVector3D;
typedef aiMatrix4x4t<float> aiMatrix4x4;

struct aiMesh;


namespace cs::assimp
{

csColor4f ConvertRGBA(aiColor4D &v);
csVector2f Convert2f(aiVector3D &v);
csVector3f Convert3f(aiVector3D &v);
csMatrix4f ConvertMatrix4x4(aiMatrix4x4 &aiMat);

iRenderMesh *ConvertRenderMesh(aiMesh *mesh, const csMatrix4f &matrix, csSkeleton* skeleton);

}