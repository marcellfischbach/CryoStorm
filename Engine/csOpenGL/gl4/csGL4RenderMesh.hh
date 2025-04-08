
#pragma once

#include <csOpenGL/csOpenGLExport.hh>

#include <csCore/csClass.hh>
#include <csCore/csTypes.hh>
#include <csCore/math/csBoundingBox.hh>
#include <csCore/math/csVector.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/graphics/eDataType.hh>
#include <csCore/graphics/ePrimitiveType.hh>
#include <csCore/graphics/csVertexDeclaration.hh>
#include <vector>

namespace cs::opengl
{

class csGL4VertexBuffer;
class csGL4IndexBuffer;

CS_CLASS()
class CS_OGL_API csGL4RenderMeshModifier : public iRenderMeshModifier
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4RenderMeshModifier(uint32_t streamID,
                          const csVertexDeclaration &declaration,
                          csGL4VertexBuffer *vertexBuffer);
  ~csGL4RenderMeshModifier() override;

  bool Map();
  void Update(eVertexStream stream, csVector2f *streamData) override;
  void Update(eVertexStream stream, csVector3f *streamData) override;
  void Update(eVertexStream stream, csVector4f *streamData) override;
  void Update(eVertexStream stream, csColor4f *streamData) override;
  void Update(eVertexStream stream, csVector4i *streamData) override;
  void Finish() override;

private:
  uint8_t *GetStreamBuffer(eVertexStream stream);

  void Unmap();
  uint32_t                 m_streamID;
  void                     *m_buffer;
  size_t                   m_bufferSize;
  size_t                   m_bufferCount;
  uint16_t                 m_streamStride;
  csVertexDeclaration      m_vertexDeclaration;
  csRef<csGL4VertexBuffer> m_vertexBuffer;
};

CS_CLASS()
class CS_OGL_API csGL4RenderMesh : public CS_SUPER(iRenderMesh)
{
  friend class csGL4RenderMeshGenerator;
  friend class csGL4RenderMeshBatchGenerator;
CS_CLASS_GEN_OBJECT;
public:
  csGL4RenderMesh(uint32_t vao,
                  const csVertexDeclaration &vd,
                  Size vertexCount,
                  csGL4VertexBuffer *vb,
                  csGL4IndexBuffer *ib,
                  ePrimitiveType type,
                  eDataType indexType,
                  Size count,
                  const csBoundingBox &boundingBox);
  ~csGL4RenderMesh() override;

  CS_NODISCARD Size GetNumberOfIndices() const;
  CS_NODISCARD Size GetNumberOfVertices() const;

  CS_NODISCARD const csBoundingBox &GetBoundingBox() const override;
  CS_NODISCARD const csVertexDeclaration &GetVertexDeclaration() const override;

  CS_NODISCARD csOwned<iRenderMeshModifier> Modify() override;

  void Render(iDevice *graphics, eRenderPass pass) override;
#if _DEBUG
  CS_NODISCARD virtual Size GetNumberOfTriangles() const override;
#endif

private:

  uint32_t                 m_vao;
  csVertexDeclaration      m_vertexDeclaration;
  csRef<csGL4VertexBuffer> m_vertexBuffer;
  csRef<csGL4IndexBuffer>  m_indexBuffer;
  csBoundingBox            m_boundingBox;

  uint32_t  m_indexType;
  eDataType m_indexDataType;
  uint32_t  m_primType;
  Size      m_count;
  Size      m_vertexCount;
};


CS_CLASS()
class CS_OGL_API csGL4RenderMeshGenerator : public CS_SUPER(iRenderMeshGenerator)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4RenderMeshGenerator(bool compatMode);
  ~csGL4RenderMeshGenerator() override;

  void SetPrimitiveType(ePrimitiveType primitiveType);
  void SetVertices(const std::vector<csVector2f> &vertices) override;
  void SetVertices(const std::vector<csVector3f> &vertices) override;
  void SetVertices(const std::vector<csVector4f> &vertices) override;
  void SetNormals(const std::vector<csVector3f> &normals) override;
  void SetColors(const std::vector<csColor4f> &colors) override;
  void SetTangents(const std::vector<csVector3f> &tangents) override;
  void SetUV0(const std::vector<csVector3f> &uv) override;
  void SetUV1(const std::vector<csVector2f> &uv) override;
  void SetUV2(const std::vector<csVector2f> &uv) override;
  void SetUV3(const std::vector<csVector2f> &uv) override;
  void SetIndices(const std::vector<uint16_t> &indices) override;
  void SetIndices(const std::vector<uint32_t> &indices) override;
  void SetUV0(const std::vector<csVector2f> &uv) override;
  void SetBoneIndices(const std::vector<csVector4i> &boneIndices) override;
  void SetBoneWeights(const std::vector<csVector4f> &boneWeights) override;
  void AddVertices(const std::vector<csVector2f> &vertices);
  void AddVertices(const std::vector<csVector3f> &vertices);
  void AddVertices(const std::vector<csVector4f> &vertices);
  void AddNormals(const std::vector<csVector3f> &normals);
  void AddColors(const std::vector<csColor4f> &colors);
  void AddTangents(const std::vector<csVector3f> &tangents);
  void AddUV0(const std::vector<csVector2f> &uv);
  void AddUV0(const std::vector<csVector3f> &uv);
  void AddUV1(const std::vector<csVector2f> &uv);
  void AddUV2(const std::vector<csVector2f> &uv);
  void AddUV3(const std::vector<csVector2f> &uv);
  void AddBoneIndices(const std::vector<csVector4i> &boneIndices);
  void AddBoneWeights(const std::vector<csVector4f> &boneWeights);
  void AddIndices(const std::vector<uint32_t> &indices);

  CS_NODISCARD size_t GetNumberOfVertices() const;
  csOwned<iRenderMesh> Generate() override;
private:
  bool                    m_compatMode;
  ePrimitiveType          m_primitiveType;
  std::vector<csVector2f> m_vertices2;
  std::vector<csVector3f> m_vertices3;
  std::vector<csVector4f> m_vertices4;
  std::vector<csVector3f> m_normals;
  std::vector<csColor4f>  m_colors;
  std::vector<csVector3f> m_tangents;
  std::vector<csVector2f> m_uv02;
  std::vector<csVector3f> m_uv03;
  std::vector<csVector2f> m_uv1;
  std::vector<csVector2f> m_uv2;
  std::vector<csVector2f> m_uv3;
  std::vector<csVector4i> m_boneIndices;
  std::vector<csVector4f> m_boneWeights;
  std::vector<uint32_t>   m_indices;


};

CS_CLASS()
class CS_OGL_API csGL4RenderMeshGeneratorFactory : public CS_SUPER(iRenderMeshGeneratorFactory)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4RenderMeshGeneratorFactory(bool compatMode);
  ~csGL4RenderMeshGeneratorFactory() override = default;


  CS_NODISCARD csOwned<iRenderMeshGenerator> Create() override;

private:
  bool m_compatMode;

};


CS_CLASS(Virtual)
class CS_OGL_API csGL4RenderMeshBatchGenerator : public CS_SUPER(iRenderMeshBatchGenerator)
{
CS_CLASS_GEN_OBJECT;
public:
  explicit csGL4RenderMeshBatchGenerator(bool compatMode);
  ~csGL4RenderMeshBatchGenerator() override;

  void Add(const iRenderMesh *mesh, const csMatrix4f &matrix) override;

  csOwned<iRenderMesh> Generate() override;

private:
  csRef<csGL4RenderMeshGenerator> m_generator;
};


CS_CLASS(Virtual)
class CS_OGL_API csGL4RenderMeshBatchGeneratorFactory : public CS_SUPER(iRenderMeshBatchGeneratorFactory)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4RenderMeshBatchGeneratorFactory(bool compatMode);
  ~csGL4RenderMeshBatchGeneratorFactory() override = default;

  CS_NODISCARD csOwned<iRenderMeshBatchGenerator> Create() override;
private:
  bool m_compatMode;

};


}