
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/math/csBoundingBox.hh>
#include <csCore/math/csVector.hh>
#include <csCore/math/csColor4f.hh>
#include <csCore/graphics/ePrimitiveType.hh>
#include <csCore/graphics/eRenderPass.hh>
#include <vector>

namespace cs
{

struct iDevice;
class csVertexDeclaration;

CS_CLASS(jclass="org.crimsonedge.core.graphics.IRenderMesh")
struct CS_CORE_API iRenderMesh : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iRenderMesh() override = default;

  CS_NODISCARD virtual const csBoundingBox &GetBoundingBox() const = 0;
  CS_NODISCARD virtual const csVertexDeclaration &GetVertexDeclaration() const = 0;

  virtual void Render(iDevice * graphics, eRenderPass pass) = 0;

#if _DEBUG
  CS_NODISCARD virtual Size GetNumberOfTriangles() const = 0;
#endif

};


CS_CLASS(jclass="org.crimsonedge.core.graphics.IRenderMeshGenerator")
struct CS_CORE_API iRenderMeshGenerator : CS_SUPER(iObject)
{
  CS_CLASS_GEN;

  ~iRenderMeshGenerator() override = default;

  virtual void SetPrimitiveType(ePrimitiveType primitiveType) = 0;
  virtual void SetVertices(const std::vector<csVector2f> & vertices) = 0;
  virtual void SetVertices(const std::vector<csVector3f> & vertices) = 0;
  virtual void SetVertices(const std::vector<csVector4f> & vertices) = 0;
  virtual void SetNormals(const std::vector<csVector3f> & normals) = 0;
  virtual void SetColors(const std::vector<csColor4f> & colors) = 0;
  virtual void SetTangents(const std::vector<csVector3f> & tangents) = 0;
  virtual void SetUV0(const std::vector<csVector2f> & uv) = 0;
  virtual void SetUV0(const std::vector<csVector3f> & uv) = 0;
  virtual void SetUV1(const std::vector<csVector2f> & uv) = 0;
  virtual void SetUV2(const std::vector<csVector2f> & uv) = 0;
  virtual void SetUV3(const std::vector<csVector2f> & uv) = 0;
  virtual void SetBoneIndices(const std::vector<csVector4i> & boneIndices) = 0;
  virtual void SetBoneWeights(const std::vector<csVector4f> & boneWeights) = 0;
  virtual void SetIndices(const std::vector<uint32_t> & indices) = 0;
  CS_NODISCARD virtual iRenderMesh* Generate() = 0;
};


CS_CLASS(jclass="org.crimsonedge.core.graphics.IRenderMeshGeneratorFactory")
struct CS_CORE_API iRenderMeshGeneratorFactory : CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iRenderMeshGeneratorFactory() override = default;

  CS_NODISCARD virtual iRenderMeshGenerator* Create() = 0;
};



CS_CLASS()
struct CS_CORE_API iRenderMeshBatchGenerator : CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iRenderMeshBatchGenerator() override = default;

  virtual void Add(const iRenderMesh* mesh, const csMatrix4f &matrix) = 0;

  CS_NODISCARD virtual iRenderMesh* Generate() = 0;
};


CS_CLASS()
struct CS_CORE_API iRenderMeshBatchGeneratorFactory : CS_SUPER(iObject)
{
CS_CLASS_GEN;
  ~iRenderMeshBatchGeneratorFactory() override = default;

  CS_NODISCARD virtual iRenderMeshBatchGenerator* Create() = 0;
};


}