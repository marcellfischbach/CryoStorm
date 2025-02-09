//
// Created by Marcell on 19.12.2024.
//

#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/csRef.hh>
#include <csCore/math/csVector2f.hh>
#include <csCore/math/csVector3f.hh>
#include <vector>

namespace cs
{

struct iRenderMesh;
class CS_CORE_API csCubeMeshGenerator
{
public:
  csCubeMeshGenerator();

  csCubeMeshGenerator &HalfExtends(const csVector3f &halfExtends);
  csCubeMeshGenerator &Smooth(bool smooth);
  csCubeMeshGenerator &GenerateTexCoords(bool generateTexCoords);
  csCubeMeshGenerator &TexScale(const csVector2f &texScale);


  csOwned<iRenderMesh> Generate();

private:
  csOwned<iRenderMesh> GenerateSmooth();
  csOwned<iRenderMesh> GenerateFlat();

  void AddFlatFaceVertices(const csVector3f &v0, const csVector3f &v1, const csVector3f &v2, const csVector3f &v3);
  void AddFlatFaceNormals(const csVector3f &normal);
  void AddFlatFaceTangents(const csVector3f &tangents);
  void AddFaceTexCoords();
  void AddFlatFaceIndices(uint32_t &offset);

  csVector3f m_halfExtends;

  bool m_smooth;
  bool m_generateTexCoords;
  csVector2f m_texScale;

  std::vector<csVector3f> m_vertices;
  std::vector<csVector3f> m_normals;
  std::vector<csVector3f> m_tangents;
  std::vector<csVector2f> m_texCoords;
  std::vector<uint32_t> m_indices;
};

}