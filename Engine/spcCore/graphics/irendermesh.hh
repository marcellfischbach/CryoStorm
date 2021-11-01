
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/math/boundingbox.hh>
#include <spcCore/math/vector2f.hh>
#include <spcCore/math/vector3f.hh>
#include <spcCore/math/vector4f.hh>
#include <spcCore/math/color4f.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <vector>

namespace spc
{

struct iDevice;
class VertexDeclaration;

SPC_CLASS()
struct SPC_CORE_API iRenderMesh : SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  ~iRenderMesh() override = default;

  SPC_NODISCARD virtual const BoundingBox &GetBoundingBox() const = 0;
  SPC_NODISCARD virtual const VertexDeclaration &GetVertexDeclaration () const = 0;

  virtual void Render(iDevice * graphics, eRenderPass pass) = 0;

#if _DEBUG
  SPC_NODISCARD virtual Size GetNumberOfTriangles() const = 0;
#endif

};


SPC_CLASS()
struct SPC_CORE_API iRenderMeshGenerator : SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

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
  SPC_NODISCARD virtual iRenderMesh* Generate() = 0;
};


SPC_CLASS()
struct SPC_CORE_API iRenderMeshGeneratorFactory : SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  ~iRenderMeshGeneratorFactory() override = default;

  SPC_NODISCARD virtual iRenderMeshGenerator* Create() = 0;
};



SPC_CLASS()
struct SPC_CORE_API iRenderMeshBatchGenerator : SPC_SUPER(iObject)
{
SPC_CLASS_GEN;

  ~iRenderMeshBatchGenerator() override = default;

  virtual void Add(const iRenderMesh* mesh, const Matrix4f &matrix) = 0;

  SPC_NODISCARD virtual iRenderMesh* Generate() = 0;
};


SPC_CLASS()
struct SPC_CORE_API iRenderMeshBatchGeneratorFactory : SPC_SUPER(iObject)
{
SPC_CLASS_GEN;
  ~iRenderMeshBatchGeneratorFactory() override = default;

  SPC_NODISCARD virtual iRenderMeshBatchGenerator* Create() = 0;
};


}