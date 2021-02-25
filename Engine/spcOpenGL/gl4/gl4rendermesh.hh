
#pragma once
#include <spcOpenGL/openglexport.hh>

#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/math/boundingbox.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/edatatype.hh>
#include <spcCore/graphics/eprimitivetype.hh>
#include <vector>

namespace spc::opengl
{

class GL4VertexBuffer;
class GL4IndexBuffer;

SPC_CLASS()
class SPC_OGL_API GL4RenderMesh : public SPC_SUPER(iRenderMesh)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMesh(UInt32 vao, GL4VertexBuffer *vb, GL4IndexBuffer *ib, ePrimitiveType type, eDataType indexType, Size count, const BoundingBox &boundingBox);
  virtual ~GL4RenderMesh();

  const BoundingBox &GetBoundingBox() const override;

  void Render(iDevice * graphics, eRenderPass pass) override;

private:

  UInt32 m_vao;
  GL4VertexBuffer* m_vertexBuffer;
  GL4IndexBuffer* m_indexBuffer;
  BoundingBox m_boundingBox;

  UInt32 m_indexType;
  UInt32 m_primType;
  Size m_count;
};



SPC_CLASS()
class SPC_OGL_API GL4RenderMeshGenerator : public SPC_SUPER(iRenderMeshGenerator)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshGenerator();
  virtual ~GL4RenderMeshGenerator();

  virtual void SetVertices(const std::vector<Vector2f> & vertices);
  virtual void SetVertices(const std::vector<Vector3f> & vertices);
  virtual void SetVertices(const std::vector<Vector4f> & vertices);
  virtual void SetNormals(const std::vector<Vector3f> & normals);
  virtual void SetColors(const std::vector<Color4f> & colors);
  virtual void SetTangents(const std::vector<Vector3f> & tangents);
  virtual void SetUV0(const std::vector<Vector2f> & uv);
  virtual void SetUV1(const std::vector<Vector2f> & uv);
  virtual void SetUV2(const std::vector<Vector2f> & uv);
  virtual void SetUV3(const std::vector<Vector2f> & uv);
  virtual void SetIndices(const std::vector<UInt32> & indices);
  virtual iRenderMesh* Generate();
private:
  std::vector<Vector2f> m_vertices2;
  std::vector<Vector3f> m_vertices3;
  std::vector<Vector4f> m_vertices4;
  std::vector<Vector3f> m_normals;
  std::vector<Color4f> m_colors;
  std::vector<Vector3f> m_tangents;
  std::vector<Vector2f> m_uv0;
  std::vector<Vector2f> m_uv1;
  std::vector<Vector2f> m_uv2;
  std::vector<Vector2f> m_uv3;
  std::vector<UInt32> m_indices;


};


SPC_CLASS()
class SPC_OGL_API GL4RenderMeshGeneratorFactory : public SPC_SUPER(iRenderMeshGeneratorFactory)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshGeneratorFactory();
  virtual ~GL4RenderMeshGeneratorFactory();

  virtual iRenderMeshGenerator* Create();

};


}