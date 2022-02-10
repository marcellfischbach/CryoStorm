
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/math/boundingbox.hh>
#include <ceCore/math/vector2f.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/math/vector4f.hh>
#include <ceCore/math/color4f.hh>
#include <ceCore/graphics/erenderpass.hh>
#include <vector>

namespace ce
{

struct iDevice;
class VertexDeclaration;

CE_CLASS()
struct CE_CORE_API iRenderMesh : CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  ~iRenderMesh() override = default;

  CE_NODISCARD virtual const BoundingBox &GetBoundingBox() const = 0;
  CE_NODISCARD virtual const VertexDeclaration &GetVertexDeclaration () const = 0;

  virtual void Render(iDevice * graphics, eRenderPass pass) = 0;

#if _DEBUG
  CE_NODISCARD virtual Size GetNumberOfTriangles() const = 0;
#endif

};


CE_CLASS()
struct CE_CORE_API iRenderMeshGenerator : CE_SUPER(iObject)
{
  CE_CLASS_GEN;

  ~iRenderMeshGenerator() override = default;

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
  virtual void SetIndices(const std::vector<uint32_t> & indices) = 0;
  CE_NODISCARD virtual iRenderMesh* Generate() = 0;
};


CE_CLASS()
struct CE_CORE_API iRenderMeshGeneratorFactory : CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  ~iRenderMeshGeneratorFactory() override = default;

  CE_NODISCARD virtual iRenderMeshGenerator* Create() = 0;
};



CE_CLASS()
struct CE_CORE_API iRenderMeshBatchGenerator : CE_SUPER(iObject)
{
CE_CLASS_GEN;

  ~iRenderMeshBatchGenerator() override = default;

  virtual void Add(const iRenderMesh* mesh, const Matrix4f &matrix) = 0;

  CE_NODISCARD virtual iRenderMesh* Generate() = 0;
};


CE_CLASS()
struct CE_CORE_API iRenderMeshBatchGeneratorFactory : CE_SUPER(iObject)
{
CE_CLASS_GEN;
  ~iRenderMeshBatchGeneratorFactory() override = default;

  CE_NODISCARD virtual iRenderMeshBatchGenerator* Create() = 0;
};


}