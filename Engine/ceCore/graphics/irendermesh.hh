
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/math/boundingbox.hh>
#include <ceCore/math/vector.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/graphics/eprimitivetype.hh>
#include <ceCore/graphics/erenderpass.hh>
#include <vector>

namespace cryo
{

struct iDevice;
class VertexDeclaration;

CS_CLASS(jclass="org.crimsonedge.core.graphics.IRenderMesh")
struct CS_CORE_API iRenderMesh : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iRenderMesh() override = default;

  CS_NODISCARD virtual const BoundingBox &GetBoundingBox() const = 0;
  CS_NODISCARD virtual const VertexDeclaration &GetVertexDeclaration () const = 0;

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
  virtual void SetVertices(const std::vector<Vector2f> & vertices) = 0;
  virtual void SetVertices(const std::vector<Vector3f> & vertices) = 0;
  virtual void SetVertices(const std::vector<Vector4f> & vertices) = 0;
  virtual void SetNormals(const std::vector<Vector3f> & normals) = 0;
  virtual void SetColors(const std::vector<Color4f> & colors) = 0;
  virtual void SetTangents(const std::vector<Vector3f> & tangents) = 0;
  virtual void SetUV0(const std::vector<Vector2f> & uv) = 0;
  virtual void SetUV0(const std::vector<Vector3f> & uv) = 0;
  virtual void SetUV1(const std::vector<Vector2f> & uv) = 0;
  virtual void SetUV2(const std::vector<Vector2f> & uv) = 0;
  virtual void SetUV3(const std::vector<Vector2f> & uv) = 0;
  virtual void SetBoneIndices(const std::vector<Vector4i> & boneIndices) = 0;
  virtual void SetBoneWeights(const std::vector<Vector4f> & boneWeights) = 0;
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

  virtual void Add(const iRenderMesh* mesh, const Matrix4f &matrix) = 0;

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