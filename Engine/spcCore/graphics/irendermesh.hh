
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

SPC_CLASS()
struct SPC_CORE_API iRenderMesh : SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  virtual ~iRenderMesh() { }

  virtual const BoundingBox &GetBoundingBox() const = 0;

  virtual void Render(iDevice * graphics, eRenderPass pass) = 0;

};


SPC_CLASS()
struct SPC_CORE_API iRenderMeshGenerator : SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;

  virtual ~iRenderMeshGenerator() { }

  virtual void SetVertices(const std::vector<Vector2f> & vertices) = 0;
  virtual void SetVertices(const std::vector<Vector3f> & vertices) = 0;
  virtual void SetVertices(const std::vector<Vector4f> & vertices) = 0;
  virtual void SetNormals(const std::vector<Vector3f> & normals) = 0;
  virtual void SetColors(const std::vector<Color4f> & colors) = 0;
  virtual void SetTangents(const std::vector<Vector3f> & tangents) = 0;
  virtual void SetUV0(const std::vector<Vector2f> & uv) = 0;
  virtual void SetUV1(const std::vector<Vector2f> & uv) = 0;
  virtual void SetUV2(const std::vector<Vector2f> & uv) = 0;
  virtual void SetUV3(const std::vector<Vector2f> & uv) = 0;
  virtual void SetIndices(const std::vector<UInt32> & indices) = 0;
  virtual iRenderMesh* Generate() = 0;
};


SPC_CLASS()
struct SPC_CORE_API iRenderMeshGeneratorFactory : SPC_SUPER(iObject)
{
  SPC_CLASS_GEN;
  virtual ~iRenderMeshGeneratorFactory() { }

  virtual iRenderMeshGenerator* Create() = 0;
};


}