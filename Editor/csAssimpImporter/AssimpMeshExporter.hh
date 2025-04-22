//
// Created by mcel on 04.03.2025.
//

#pragma once

#include <assimp/scene.h>
#include <map>
#include <set>
#include <vector>
#include <csAssimpImporter/AssimpSkeletonExporter.hh>
#include <csCore/math/csMatrix.hh>

namespace cs::imp
{

class AssimpMeshExporter
{
public:
  AssimpMeshExporter(const aiScene *scene);

  void combine(const aiNode *node);

  void Export(const std::string &filename, const std::string &referenceName) const;

  void SetSkeletonRootNames(const std::set<std::string>& rootNames);

private:
  const aiScene *m_scene;

  void combine(const aiNode *node, const aiMatrix4x4 &parent);

  enum VDataType
  {
    END,
    VERTEX,
    NORMAL,
    TANGENT,
    COLOR0,
    COLOR1,
    BONE_ID,
    BONE_WEIGHT,
    TEX_COORD0_1,
    TEX_COORD0_2,
    TEX_COORD0_3,
    TEX_COORD1_1,
    TEX_COORD1_2,
    TEX_COORD1_3,
    TEX_COORD2_1,
    TEX_COORD2_2,
    TEX_COORD2_3
  };

  enum PrimitiveType
  {
    POINTS,
    LINES,
    TRIANGLES,
  };

  enum IndexType
  {
    UINT16,
    UINT32
  };

  typedef std::set<VDataType> VertexDecl;


  struct BoneWeight
  {
    uint32_t boneId;
    float    weight;
  };

  struct VertexBoneData
  {
    std::vector<BoneWeight> weights;
    void Normalize ();
  };

  struct MeshData
  {

    VertexDecl    vertexDecl;
    uint32_t      materialIdx;
    PrimitiveType primitiveType;

    std::vector<aiVector3D>     position;
    std::vector<aiVector3D>     normal;
    std::vector<aiVector3D>     tangent;
    std::vector<aiColor4D>      color0;
    std::vector<aiColor4D>      color1;
    uint32_t                    texCoord0Size;
    std::vector<aiVector3D>     texCoord0;
    uint32_t                    texCoord1Size;
    std::vector<aiVector3D>     texCoord1;
    uint32_t                    texCoord2Size;
    std::vector<aiVector3D>     texCoord2;
    std::vector<VertexBoneData> boneData;


    std::vector<uint32_t> indices;
  };


  AssimpSkeletonExporter m_skeletonExporter;

  std::vector<MeshData> m_meshData;
  uint32_t m_boneId = 0;
  std::map<std::string, uint32_t> m_boneNames;

  std::map<std::string, csMatrix4f> m_matrices;
  void ScanMatrices (const aiNode* node, const csMatrix4f& parent);


  bool HasBoneData () const;

  void Push(MeshData &meshData, const aiMesh *mesh, const aiMatrix4x4 &matrix);
  void PushTexCoord(std::vector<aiVector3D> &values, aiVector3D *srcValues, uint32_t srcValueCount);
  void PushColor(std::vector<aiColor4D> &values, aiColor4D *srcValues, uint32_t srcValueCount);
  void PushPosition(std::vector<aiVector3D> &values,
                    const aiMatrix4x4 &matrix,
                    const aiMesh *mesh,
                    aiVector3D *srcValues,
                    uint32_t srcValueCount);
  void PushNormal(std::vector<aiVector3D> &values,
                  const aiMatrix3x3 &matrix,
                  aiVector3D *srcValues,
                  uint32_t srcValueCount);
  void PushBones(std::vector<VertexBoneData> &boneData,
                 aiBone* bone,
                 uint32_t boneId);
  bool VerifyMeshSkeletonMatchesBones(const aiMesh* mesh);

  std::set<std::string> m_skeletonRootNames;
  aiNode* FindSkeletonRootNode(aiNode* root);
  void NormalizeBoneWeights (MeshData &meshData);

  VertexDecl ExtractVertexDecl(const aiMesh *mesh);
  MeshData &FindMeshData(const VertexDecl &decl, PrimitiveType primitiveType, uint32_t materialIdx);

  void PushMesh(const aiMatrix4x4 &mat, const aiMesh *mesh);

  void WriteMesh(std::ostream &out, const MeshData &mesh) const;
  void WriteMeshBoneIndex (std::ostream  &out, const MeshData &mesh) const;

  void WriteValuesV1(std::ostream& out, uint8_t dataType, const std::vector<aiVector3D>& vertices) const;
  void WriteValuesV2(std::ostream& out, uint8_t dataType, const std::vector<aiVector3D>& vertices) const;
  void WriteValuesV3(std::ostream& out, uint8_t dataType, const std::vector<aiVector3D>& vertices) const;
  void WriteValuesC4(std::ostream& out, uint8_t dataType, const std::vector<aiColor4D>& colors) const;
  void WriteBoneIDs(std::ostream& out, const std::vector<VertexBoneData>& boneData) const;
  void WriteBoneWeights(std::ostream& out, const std::vector<VertexBoneData>& boneData) const;
};

}
