//
// Created by mcel on 04.03.2025.
//

#pragma once

#include <assimp/scene.h>
#include <map>
#include <set>
#include <vector>

namespace cs::imp
{

class AssimpMeshExporter
{
public:
  AssimpMeshExporter(const aiScene* scene);

  void combine(const aiMesh *mesh);
  void combine(const aiNode *node);

  void Export(const std::string &filename, const std::string &referenceName) const;


private:
  const aiScene* m_scene;

  void combine(const aiNode *node, const aiMatrix4x4 &parent);

  enum VDataType
  {
    END,
    VERTEX,
    NORMAL,
    TANGENT,
    COLOR0,
    COLOR1,
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

  struct MeshData
  {

    VertexDecl vertexDecl;
    uint32_t materialIdx;
    PrimitiveType primitiveType;

    std::vector<aiVector3D> position;
    std::vector<aiVector3D> normal;
    std::vector<aiVector3D> tangent;
    std::vector<aiColor4D>  color0;
    std::vector<aiColor4D>  color1;
    uint32_t                texCoord0Size;
    std::vector<aiVector3D> texCoord0;
    uint32_t                texCoord1Size;
    std::vector<aiVector3D> texCoord1;
    uint32_t                texCoord2Size;
    std::vector<aiVector3D> texCoord2;

    std::vector<uint32_t>   indices;
  };

  std::vector<MeshData> m_meshData;

  void Push (MeshData &meshData, const aiMesh* mesh, const aiMatrix4x4 &matrix);

  VertexDecl ExtractVertexDecl (const aiMesh* mesh);
  MeshData &FindMeshData (const VertexDecl &decl, PrimitiveType primitiveType, uint32_t materialIdx);

  void PushMesh(const aiMatrix4x4 &mat, const aiMesh *mesh);

  void WriteMesh(std::ostream& out, const MeshData &mesh) const;

};

}
