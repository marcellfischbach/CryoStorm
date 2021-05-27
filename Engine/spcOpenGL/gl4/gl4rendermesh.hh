
#pragma once
#include <spcOpenGL/openglexport.hh>

#include <spcCore/class.hh>
#include <spcCore/types.hh>
#include <spcCore/math/boundingbox.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/edatatype.hh>
#include <spcCore/graphics/eprimitivetype.hh>
#include <spcCore/graphics/vertexdeclaration.hh>
#include <vector>

namespace spc::opengl
{

class GL4VertexBuffer;
class GL4IndexBuffer;

SPC_CLASS()
class SPC_OGL_API GL4RenderMesh : public SPC_SUPER(iRenderMesh)
{
  friend class GL4RenderMeshGenerator;
  friend class GL4RenderMeshBatchGenerator;
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMesh(UInt32 vao, const VertexDeclaration &vd, GL4VertexBuffer *vb, GL4IndexBuffer *ib, ePrimitiveType type, eDataType indexType, Size count, const BoundingBox &boundingBox);
  ~GL4RenderMesh() override;

  SPC_NODISCARD Size GetNumberOfIndices() const;
  SPC_NODISCARD Size GetNumberOfVertices() const;

  SPC_NODISCARD const BoundingBox &GetBoundingBox() const override;
  SPC_NODISCARD const VertexDeclaration& GetVertexDeclaration() const override;

  void Render(iDevice * graphics, eRenderPass pass) override;
#if _DEBUG
  SPC_NODISCARD virtual Size GetNumberOfTriangles() const override;
#endif

private:

  UInt32 m_vao;
  VertexDeclaration m_vertexDeclaration;
  GL4VertexBuffer* m_vertexBuffer;
  GL4IndexBuffer* m_indexBuffer;
  BoundingBox m_boundingBox;

  UInt32 m_indexType;
  eDataType m_indexDataType;
  UInt32 m_primType;
  Size m_count;
  Size m_vertexCount;
};



SPC_CLASS()
class SPC_OGL_API GL4RenderMeshGenerator : public SPC_SUPER(iRenderMeshGenerator)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshGenerator();
  ~GL4RenderMeshGenerator() override;

  void SetVertices(const std::vector<Vector2f> & vertices) override;
  void SetVertices(const std::vector<Vector3f> & vertices) override;
  void SetVertices(const std::vector<Vector4f> & vertices) override;
  void SetNormals(const std::vector<Vector3f> & normals) override;
  void SetColors(const std::vector<Color4f> & colors) override;
  void SetTangents(const std::vector<Vector3f> & tangents) override;
  void SetUV0(const std::vector<Vector3f> & uv) override;
  void SetUV1(const std::vector<Vector2f> & uv) override;
  void SetUV2(const std::vector<Vector2f> & uv) override;
  void SetUV3(const std::vector<Vector2f> & uv) override;
  void SetIndices(const std::vector<UInt32> & indices) override;
  void SetUV0(const std::vector<Vector2f> & uv) override;
  void AddVertices(const std::vector<Vector2f> & vertices);
  void AddVertices(const std::vector<Vector3f> & vertices);
  void AddVertices(const std::vector<Vector4f> & vertices);
  void AddNormals(const std::vector<Vector3f> & normals);
  void AddColors(const std::vector<Color4f> & colors);
  void AddTangents(const std::vector<Vector3f> & tangents);
  void AddUV0(const std::vector<Vector2f> & uv);
  void AddUV0(const std::vector<Vector3f> & uv);
  void AddUV1(const std::vector<Vector2f> & uv);
  void AddUV2(const std::vector<Vector2f> & uv);
  void AddUV3(const std::vector<Vector2f> & uv) ;
  void AddIndices(const std::vector<UInt32> & indices);

  SPC_NODISCARD size_t GetNumberOfVertices() const;
  iRenderMesh* Generate() override;
private:
  std::vector<Vector2f> m_vertices2;
  std::vector<Vector3f> m_vertices3;
  std::vector<Vector4f> m_vertices4;
  std::vector<Vector3f> m_normals;
  std::vector<Color4f> m_colors;
  std::vector<Vector3f> m_tangents;
  std::vector<Vector2f> m_uv02;
  std::vector<Vector3f> m_uv03;
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
  ~GL4RenderMeshGeneratorFactory() override = default;

  SPC_NODISCARD iRenderMeshGenerator* Create() override;

};


SPC_CLASS()
class SPC_OGL_API GL4RenderMeshBatchGenerator : public SPC_SUPER(iRenderMeshBatchGenerator)
{
SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshBatchGenerator() = default;
  ~GL4RenderMeshBatchGenerator() override = default;

  void Add(const iRenderMesh* mesh, const Matrix4f &matrix) override;

   iRenderMesh* Generate() override;

private:
  GL4RenderMeshGenerator m_generator;
};



SPC_CLASS()
class SPC_OGL_API GL4RenderMeshBatchGeneratorFactory : public SPC_SUPER(iRenderMeshBatchGeneratorFactory)
{
SPC_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshBatchGeneratorFactory();
  ~GL4RenderMeshBatchGeneratorFactory() override = default;

  SPC_NODISCARD iRenderMeshBatchGenerator* Create() override;

};



}