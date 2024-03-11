
#pragma once
#include <ceOpenGL/openglexport.hh>

#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/math/boundingbox.hh>
#include <ceCore/math/vector.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/graphics/edatatype.hh>
#include <ceCore/graphics/eprimitivetype.hh>
#include <ceCore/graphics/vertexdeclaration.hh>
#include <vector>

namespace ce::opengl
{

class GL4VertexBuffer;
class GL4IndexBuffer;

CE_CLASS()
class CE_OGL_API GL4RenderMesh : public CE_SUPER(iRenderMesh)
{
  friend class GL4RenderMeshGenerator;
  friend class GL4RenderMeshBatchGenerator;
  CE_CLASS_GEN_OBJECT;
public:
  GL4RenderMesh(uint32_t vao,
                const VertexDeclaration &vd,
                Size vertexCount,
                GL4VertexBuffer *vb,
                GL4IndexBuffer *ib,
                ePrimitiveType type,
                eDataType indexType,
                Size count,
                const BoundingBox &boundingBox);
  ~GL4RenderMesh() override;

  CE_NODISCARD Size GetNumberOfIndices() const;
  CE_NODISCARD Size GetNumberOfVertices() const;

  CE_NODISCARD const BoundingBox &GetBoundingBox() const override;
  CE_NODISCARD const VertexDeclaration& GetVertexDeclaration() const override;

  void Render(iDevice * graphics, eRenderPass pass) override;
#if _DEBUG
  CE_NODISCARD virtual Size GetNumberOfTriangles() const override;
#endif

private:

  uint32_t m_vao;
  VertexDeclaration m_vertexDeclaration;
  GL4VertexBuffer* m_vertexBuffer;
  GL4IndexBuffer* m_indexBuffer;
  BoundingBox m_boundingBox;

  uint32_t m_indexType;
  eDataType m_indexDataType;
  uint32_t m_primType;
  Size m_count;
  Size m_vertexCount;
};



CE_CLASS()
class CE_OGL_API GL4RenderMeshGenerator : public CE_SUPER(iRenderMeshGenerator)
{
  CE_CLASS_GEN_OBJECT;
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
  void SetIndices(const std::vector<uint32_t> & indices) override;
  void SetUV0(const std::vector<Vector2f> & uv) override;
  void SetBoneIndices(const std::vector<Vector4i> &boneIndices) override;
  void SetBoneWeights(const std::vector<Vector4f> &boneWeights) override;
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
  void AddBoneIndices(const std::vector<Vector4i> & boneIndices) ;
  void AddBoneWeights(const std::vector<Vector4f> & boneWeights) ;
  void AddIndices(const std::vector<uint32_t> & indices);

  CE_NODISCARD size_t GetNumberOfVertices() const;
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
  std::vector<Vector4i> m_boneIndices;
  std::vector<Vector4f> m_boneWeights;
  std::vector<uint32_t> m_indices;


};


CE_CLASS()
class CE_OGL_API GL4RenderMeshGeneratorFactory : public CE_SUPER(iRenderMeshGeneratorFactory)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshGeneratorFactory();
  ~GL4RenderMeshGeneratorFactory() override = default;

  CE_NODISCARD iRenderMeshGenerator* Create() override;

};


CE_CLASS()
class CE_OGL_API GL4RenderMeshBatchGenerator : public CE_SUPER(iRenderMeshBatchGenerator)
{
CE_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshBatchGenerator() = default;
  ~GL4RenderMeshBatchGenerator() override = default;

  void Add(const iRenderMesh* mesh, const Matrix4f &matrix) override;

   iRenderMesh* Generate() override;

private:
  GL4RenderMeshGenerator m_generator;
};



CE_CLASS()
class CE_OGL_API GL4RenderMeshBatchGeneratorFactory : public CE_SUPER(iRenderMeshBatchGeneratorFactory)
{
CE_CLASS_GEN_OBJECT;
public:
  GL4RenderMeshBatchGeneratorFactory();
  ~GL4RenderMeshBatchGeneratorFactory() override = default;

  CE_NODISCARD iRenderMeshBatchGenerator* Create() override;

};



}