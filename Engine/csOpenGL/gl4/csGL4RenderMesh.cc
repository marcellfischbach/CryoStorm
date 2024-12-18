

#include <csOpenGL/gl4/csGL4RenderMesh.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4VertexBuffer.hh>
#include <csOpenGL/gl4/csGL4IndexBuffer.hh>
#include <csOpenGL/csGLError.hh>
#include <csCore/graphics/csVertexDeclaration.hh>
#include <gl/glew.h>


namespace cs::opengl
{


static GLenum PrimitiveTypeMap[] = {
    GL_POINTS,
    GL_LINES,
    GL_TRIANGLES
};

static GLenum DataTypeMap[] = {
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT,
    GL_DOUBLE
};


csGL4RenderMesh::csGL4RenderMesh(uint32_t vao,
                                 const csVertexDeclaration &vd,
                                 Size vertexCount,
                                 csGL4VertexBuffer *vb,
                                 csGL4IndexBuffer *ib,
                                 ePrimitiveType primitiveType,
                                 eDataType indexType,
                                 Size count,
                                 const csBoundingBox &boundingBox)
    : iRenderMesh()
    , m_vao(vao)
    , m_vertexDeclaration(vd)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_primType(PrimitiveTypeMap[primitiveType])
    , m_indexDataType(indexType)
    , m_indexType(DataTypeMap[indexType])
    , m_count(count)
    , m_boundingBox(boundingBox)
    , m_vertexCount(vertexCount)
{
  CS_CLASS_GEN_CONSTR;
  CS_SET(m_vertexBuffer, vb);
  CS_SET(m_indexBuffer, ib);

}

csGL4RenderMesh::~csGL4RenderMesh()
{
  if (m_vao != 0)
  {
    CS_GL_ERROR();
    glDeleteVertexArrays(1, &m_vao);
    CS_GL_ERROR();
    m_vao = 0;
  }
  CS_RELEASE(m_vertexBuffer);
  m_vertexBuffer = nullptr;

  CS_RELEASE(m_indexBuffer);
  m_indexBuffer = nullptr;
}

Size csGL4RenderMesh::GetNumberOfIndices() const
{
  return m_count;
}

Size csGL4RenderMesh::GetNumberOfVertices() const
{
  return m_vertexCount;
}

const csBoundingBox &csGL4RenderMesh::GetBoundingBox() const
{
  return m_boundingBox;
}

const csVertexDeclaration &csGL4RenderMesh::GetVertexDeclaration() const
{
  return m_vertexDeclaration;
}

void csGL4RenderMesh::Render(iDevice *graphics, eRenderPass pass)
{
  if (m_vao)
  {
    CS_GL_ERROR();
    glBindVertexArray(m_vao);
    CS_GL_ERROR();
  }
  else
  {
    m_indexBuffer->Bind();
    m_vertexBuffer->Bind();
    const std::vector<csVertexDeclaration::Attribute> &vdAttributes = m_vertexDeclaration.GetAttributes(0);
    for (const csVertexDeclaration::Attribute &attribute: vdAttributes)
    {
      CS_GL_ERROR();
      glVertexAttribPointer(attribute.Location,
                            attribute.Size,
                            DataTypeMap[attribute.Type],
                            false,
                            attribute.Stride,
                            reinterpret_cast<const void *>(attribute.Offset)
      );
      CS_GL_ERROR();
      glEnableVertexAttribArray(attribute.Location);
      CS_GL_ERROR();
    }
  }
  glDrawElements(m_primType, (GLsizei) m_count, m_indexType, nullptr);
  CS_GL_ERROR();


#if _DEBUG
  auto gl4Device = graphics->Query<csGL4Device>();
  gl4Device->IncDrawCalls();
  gl4Device->IncTriangles(GetNumberOfTriangles());
#endif
}

#if _DEBUG
Size csGL4RenderMesh::GetNumberOfTriangles() const
{
  return m_count / 3;
}
#endif

csGL4RenderMeshGenerator::csGL4RenderMeshGenerator(bool compatMode)
    : iRenderMeshGenerator()
    , m_compatMode(compatMode)
    , m_primitiveType(ePT_Triangles)
{
  CS_CLASS_GEN_CONSTR;
}

csGL4RenderMeshGenerator::~csGL4RenderMeshGenerator() = default;


void csGL4RenderMeshGenerator::SetPrimitiveType(cs::ePrimitiveType primitiveType)
{
  m_primitiveType = primitiveType;
}

void csGL4RenderMeshGenerator::SetVertices(const std::vector<csVector2f> &vertices)
{
  m_vertices2 = vertices;
}

void csGL4RenderMeshGenerator::SetVertices(const std::vector<csVector3f> &vertices)
{
  m_vertices3 = vertices;
}

void csGL4RenderMeshGenerator::SetVertices(const std::vector<csVector4f> &vertices)
{
  m_vertices4 = vertices;
}


void csGL4RenderMeshGenerator::SetNormals(const std::vector<csVector3f> &normals)
{
  m_normals = normals;
}

void csGL4RenderMeshGenerator::SetColors(const std::vector<csColor4f> &colors)
{
  m_colors = colors;
}

void csGL4RenderMeshGenerator::SetTangents(const std::vector<csVector3f> &tangents)
{
  m_tangents = tangents;
}

void csGL4RenderMeshGenerator::SetUV0(const std::vector<csVector2f> &uv)
{
  m_uv02 = uv;
}

void csGL4RenderMeshGenerator::SetUV0(const std::vector<csVector3f> &uv)
{
  m_uv03 = uv;
}

void csGL4RenderMeshGenerator::SetUV1(const std::vector<csVector2f> &uv)
{
  m_uv1 = uv;
}

void csGL4RenderMeshGenerator::SetUV2(const std::vector<csVector2f> &uv)
{
  m_uv2 = uv;
}

void csGL4RenderMeshGenerator::SetUV3(const std::vector<csVector2f> &uv)
{
  m_uv3 = uv;
}

void csGL4RenderMeshGenerator::SetBoneIndices(const std::vector<csVector4i> &boneIndices)
{
  m_boneIndices = boneIndices;
}

void csGL4RenderMeshGenerator::SetBoneWeights(const std::vector<csVector4f> &boneWeights)
{
  m_boneWeights = boneWeights;
}

void csGL4RenderMeshGenerator::SetIndices(const std::vector<uint32_t> &indices)
{
  m_indices = indices;
}

void csGL4RenderMeshGenerator::AddVertices(const std::vector<csVector2f> &vertices)
{
  m_vertices2.insert(m_vertices2.end(), vertices.begin(), vertices.end());
}

void csGL4RenderMeshGenerator::AddVertices(const std::vector<csVector3f> &vertices)
{
  m_vertices3.insert(m_vertices3.end(), vertices.begin(), vertices.end());
}

void csGL4RenderMeshGenerator::AddVertices(const std::vector<csVector4f> &vertices)
{
  m_vertices4.insert(m_vertices4.end(), vertices.begin(), vertices.end());
}


void csGL4RenderMeshGenerator::AddNormals(const std::vector<csVector3f> &normals)
{
  m_normals.insert(m_normals.end(), normals.begin(), normals.end());
}

void csGL4RenderMeshGenerator::AddColors(const std::vector<csColor4f> &colors)
{
  m_colors.insert(m_colors.end(), colors.begin(), colors.end());
}

void csGL4RenderMeshGenerator::AddTangents(const std::vector<csVector3f> &tangents)
{
  m_tangents.insert(m_tangents.end(), tangents.begin(), tangents.end());
}

void csGL4RenderMeshGenerator::AddUV0(const std::vector<csVector2f> &uv)
{
  m_uv02.insert(m_uv02.end(), uv.begin(), uv.end());
}

void csGL4RenderMeshGenerator::AddUV0(const std::vector<csVector3f> &uv)
{
  m_uv03.insert(m_uv03.end(), uv.begin(), uv.end());
}

void csGL4RenderMeshGenerator::AddUV1(const std::vector<csVector2f> &uv)
{
  m_uv1.insert(m_uv1.end(), uv.begin(), uv.end());
}

void csGL4RenderMeshGenerator::AddUV2(const std::vector<csVector2f> &uv)
{
  m_uv2.insert(m_uv2.end(), uv.begin(), uv.end());
}

void csGL4RenderMeshGenerator::AddUV3(const std::vector<csVector2f> &uv)
{
  m_uv3.insert(m_uv3.end(), uv.begin(), uv.end());
}

void csGL4RenderMeshGenerator::AddBoneIndices(const std::vector<csVector4i> &boneIndices)
{
  m_boneIndices.insert(m_boneIndices.end(), boneIndices.begin(), boneIndices.end());
}

void csGL4RenderMeshGenerator::AddBoneWeights(const std::vector<csVector4f> &boneWeights)
{
  m_boneWeights.insert(m_boneWeights.end(), boneWeights.begin(), boneWeights.end());
}

void csGL4RenderMeshGenerator::AddIndices(const std::vector<uint32_t> &indices)
{
  m_indices.insert(m_indices.end(), indices.begin(), indices.end());
}


size_t csGL4RenderMeshGenerator::GetNumberOfVertices() const
{
  return std::max(m_vertices2.size(), std::max(m_vertices3.size(), m_vertices4.size()));
}

iRenderMesh *csGL4RenderMeshGenerator::Generate()
{
  if (!m_compatMode)
  {
    CS_GL_ERROR();
    glBindVertexArray(0);
    CS_GL_ERROR();
  }
  int numVertexDefs = 0;
  int numUV0Defs = 0;
  numVertexDefs += m_vertices2.empty() ? 0 : 1;
  numVertexDefs += m_vertices3.empty() ? 0 : 1;
  numVertexDefs += m_vertices4.empty() ? 0 : 1;
  numUV0Defs += m_uv02.empty() ? 0 : 1;
  numUV0Defs += m_uv03.empty() ? 0 : 1;
  if (m_indices.empty() || numVertexDefs != 1 || numUV0Defs > 1)
  {
    return nullptr;
  }

  std::vector<csVertexDeclaration::Attribute> attributes;
  uint16_t offset = 0;
  uint16_t count = 0;
  Size vertexCount = 0;
  if (!m_vertices2.empty())
  {
    attributes.emplace_back(0, eVS_Vertices, 2, eDT_Float, 0, 0);
    offset += 2 * sizeof(float);
    count += 2;
    vertexCount = m_vertices2.size();
  }
  else if (!m_vertices3.empty())
  {
    attributes.emplace_back(0, eVS_Vertices, 3, eDT_Float, 0, 0);
    offset += 3 * sizeof(float);
    count += 3;
    vertexCount = m_vertices3.size();
  }
  else if (!m_vertices4.empty())
  {
    attributes.emplace_back(0, eVS_Vertices, 4, eDT_Float, 0, 0);
    offset += 4 * sizeof(float);
    count += 4;
    vertexCount = m_vertices4.size();
  }
  if (!m_normals.empty())
  {
    if (m_normals.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_Normals, 3, eDT_Float, 0, offset);
    count += 3;
    offset += 3 * sizeof(float);
  }

  if (!m_tangents.empty())
  {
    if (m_tangents.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_Tangents, 3, eDT_Float, 0, offset);
    count += 3;
    offset += 3 * sizeof(float);
  }
  if (!m_uv02.empty())
  {
    if (m_uv02.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_UV0, 2, eDT_Float, 0, offset);
    count += 2;
    offset += 2 * sizeof(float);
  }
  else if (!m_uv03.empty())
  {
    if (m_uv03.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_UV0, 3, eDT_Float, 0, offset);
    count += 3;
    offset += 3 * sizeof(float);
  }
  if (!m_uv1.empty())
  {
    if (m_uv1.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_UV1, 2, eDT_Float, 0, offset);
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (!m_uv2.empty())
  {
    if (m_uv2.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_UV2, 2, eDT_Float, 0, offset);
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (!m_uv3.empty())
  {
    if (m_uv3.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_UV3, 2, eDT_Float, 0, offset);
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (!m_colors.empty())
  {
    if (m_colors.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_Colors, 4, eDT_Float, 0, offset);
    count += 4;
    offset += 4 * sizeof(float);
  }
  if (!m_boneIndices.empty())
  {
    if (m_boneIndices.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_BoneIndices, 4, eDT_Float, 0, offset);
    count += 4;
    offset += 4 * sizeof(float);
  }
  if (!m_boneWeights.empty())
  {
    if (m_boneWeights.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(0, eVS_BoneWeights, 4, eDT_Float, 0, offset);
    count += 4;
    offset += 4 * sizeof(float);
  }

  for (csVertexDeclaration::Attribute &attribute: attributes)
  {
    attribute.Stride = offset;
  }

  csBoundingBox bbox;
  bbox.Clear();
  csVertexDeclaration vd(attributes);
  auto vBuffer = new float[count * vertexCount];

  for (Size i = 0, c = 0; i < vertexCount; ++i)
  {
    if (!m_vertices2.empty())
    {
      csVector2f &v = m_vertices2[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      bbox.Add(csVector3f(v.x, v.y, 0.0f));
    }
    else if (!m_vertices3.empty())
    {
      csVector3f &v = m_vertices3[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      bbox.Add(v);
    }
    else if (!m_vertices4.empty())
    {
      csVector4f &v = m_vertices4[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      vBuffer[c++] = v.w;
      bbox.Add((csVector3f) v);
    }

    if (!m_normals.empty())
    {
      csVector3f &v = m_normals[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_tangents.empty())
    {
      csVector3f &v = m_tangents[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_uv02.empty())
    {
      csVector2f &v = m_uv02[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    else if (!m_uv03.empty())
    {
      csVector3f &v = m_uv03[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_uv1.empty())
    {
      csVector2f &v = m_uv1[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_uv2.empty())
    {
      csVector2f &v = m_uv2[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_uv3.empty())
    {
      csVector2f &v = m_uv3[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_colors.empty())
    {
      csColor4f &v = m_colors[i];
      vBuffer[c++] = v.r;
      vBuffer[c++] = v.g;
      vBuffer[c++] = v.b;
      vBuffer[c++] = v.a;
    }
    if (!m_boneIndices.empty())
    {
      csVector4i &v = m_boneIndices[i];
      vBuffer[c++] = (float) v.x + 0.25f;
      vBuffer[c++] = (float) v.y + 0.25f;
      vBuffer[c++] = (float) v.z + 0.25f;
      vBuffer[c++] = (float) v.w + 0.25f;
    }
    if (!m_boneWeights.empty())
    {
      csVector4f &v = m_boneWeights[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      vBuffer[c++] = v.w;
    }
  }
  bbox.Finish();

  auto vb = new csGL4VertexBuffer();
  vb->Bind();
  vb->CreateForRendering(vertexCount * offset, eBU_Static);
  vb->Copy(vBuffer, vertexCount * offset);
  delete[] vBuffer;

  auto ib = new csGL4IndexBuffer();
  eDataType indexType;
  if (vertexCount >= 65536)
  {
    ib->Bind();
    ib->CreateForRendering(m_indices.size() * sizeof(uint32_t), eBU_Static);
    ib->Copy(&m_indices[0], m_indices.size() * sizeof(uint32_t));
    indexType = eDT_UnsignedInt;
  }
  else
  {
    auto iBuffer = new uint16_t[m_indices.size()];
    for (Size i = 0, in = m_indices.size(); i < in; ++i)
    {
      iBuffer[i] = static_cast<uint16_t>(m_indices[i]);
    }
    ib->Bind();
    ib->CreateForRendering(m_indices.size() * sizeof(uint16_t), eBU_Static);
    ib->Copy(iBuffer, m_indices.size() * sizeof(uint16_t));
    delete[] iBuffer;

    indexType = eDT_UnsignedShort;
  }

  GLuint vao = 0;
  if (!m_compatMode)
  {
    CS_GL_ERROR();
    glGenVertexArrays(1, &vao);
    CS_GL_ERROR();
    glBindVertexArray(vao);
    CS_GL_ERROR();
  }

  ib->Bind();
  vb->Bind();
  if (!m_compatMode)
  {
    const std::vector<csVertexDeclaration::Attribute> &vdAttributes = vd.GetAttributes(0);
    for (const csVertexDeclaration::Attribute &attribute: vdAttributes)
    {
      CS_GL_ERROR();
      glVertexAttribPointer(
          attribute.Location,
          attribute.Size,
          DataTypeMap[attribute.Type],
          false,
          attribute.Stride,
          reinterpret_cast<const void *>(attribute.Offset)
      );
      CS_GL_ERROR();
      glEnableVertexAttribArray(attribute.Location);
      CS_GL_ERROR();
    }
    glBindVertexArray(0);
    CS_GL_ERROR();
  }

  auto mesh = new csGL4RenderMesh(
      vao,
      vd,
      vertexCount,
      vb,
      ib,
      m_primitiveType,
      indexType,
      m_indices.size(),
      bbox
  );

  ib->Release();
  vb->Release();
  return mesh;
}


csGL4RenderMeshGeneratorFactory::csGL4RenderMeshGeneratorFactory(bool compatMode)
    : iRenderMeshGeneratorFactory()
    , m_compatMode(compatMode)
{
  CS_CLASS_GEN_CONSTR;
}

iRenderMeshGenerator *csGL4RenderMeshGeneratorFactory::Create()
{
  return new csGL4RenderMeshGenerator(m_compatMode);
}


csGL4RenderMeshBatchGenerator::csGL4RenderMeshBatchGenerator(bool compatMode)
    : iRenderMeshBatchGenerator()
    , m_generator(new csGL4RenderMeshGenerator(compatMode))
{

}

csGL4RenderMeshBatchGenerator::~csGL4RenderMeshBatchGenerator()
{
  delete m_generator;
  m_generator = nullptr;
}

void csGL4RenderMeshBatchGenerator::Add(const iRenderMesh *mesh, const csMatrix4f &matrix)
{
  auto gl4Mesh = dynamic_cast<const csGL4RenderMesh *>(mesh);
  std::vector<csVector2f> vertices2;
  std::vector<csVector3f> vertices3;
  std::vector<csVector4f> vertices4;
  std::vector<csVector3f> normals;
  std::vector<csColor4f> colors;
  std::vector<csVector3f> tangents;
  std::vector<csVector2f> uv02;
  std::vector<csVector3f> uv03;
  std::vector<csVector2f> uv1;
  std::vector<csVector2f> uv2;
  std::vector<csVector2f> uv3;
  std::vector<csVector4i> boneIndices;
  std::vector<csVector4f> boneWeights;
  std::vector<uint32_t> indices;

  size_t vertexOffset = m_generator->GetNumberOfVertices();
  auto rotMat = (csMatrix3f) matrix;
  size_t numVertices = gl4Mesh->GetNumberOfVertices();
  void *buffer = nullptr;
  Size bufferSize = 0;
  gl4Mesh->m_vertexBuffer->Map(&buffer, bufferSize);
  auto *b8 = reinterpret_cast<uint8_t *>(buffer);
  for (const auto &attribute: mesh->GetVertexDeclaration().GetAttributes(0))
  {
    for (size_t i = 0; i < numVertices; i++)
    {
      switch (attribute.Location)
      {
        case eVS_Vertices:
          switch (attribute.Size)
          {
            case 2:
            {
              csVector2f v2 = *reinterpret_cast<csVector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
              vertices2.push_back((csVector2f) (matrix * csVector4f(v2, 0, 1)));
              break;
            }
            case 3:
            {
              csVector3f v3 = *reinterpret_cast<csVector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
              vertices3.push_back((csVector3f) (matrix * csVector4f(v3, 1)));
              break;
            }
            case 4:
            {
              csVector4f v4 = *reinterpret_cast<csVector4f *>(&b8[i * attribute.Stride + attribute.Offset]);
              vertices4.push_back(matrix * v4);
              break;
            }
          }
          break;
        case eVS_Normals:
        {
          csVector3f v3 = *reinterpret_cast<csVector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
          normals.push_back(rotMat * v3);
          break;
        }
        case eVS_Tangents:
        {
          csVector3f v3 = *reinterpret_cast<csVector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
          tangents.push_back(rotMat * v3);
          break;
        }
        case eVS_Colors:
        {
          csColor4f c = *reinterpret_cast<csColor4f *>(&b8[i * attribute.Stride + attribute.Offset]);
          colors.push_back(c);
          break;
        }
        case eVS_UV0:
          switch (attribute.Size)
          {
            case 2:
            {
              csVector2f v2 = *reinterpret_cast<csVector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
              uv02.push_back(v2);
              break;
            }
            case 3:
            {
              csVector3f v3 = *reinterpret_cast<csVector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
              uv03.push_back(v3);
              break;
            }
          }
          break;
        case eVS_UV1:
        {
          csVector2f v2 = *reinterpret_cast<csVector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
          uv1.push_back(v2);
          break;
        }
        case eVS_UV2:
        {
          csVector2f v2 = *reinterpret_cast<csVector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
          uv2.push_back(v2);
          break;
        }
        case eVS_UV3:
        {
          csVector2f v2 = *reinterpret_cast<csVector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
          uv3.push_back(v2);
          break;
        }
        case eVS_BoneIndices:
        {
          csVector4f idx = *reinterpret_cast<csVector4f *>(&b8[i * attribute.Stride + attribute.Offset]);
          boneIndices.emplace_back(csVector4i((int) idx.x, (int) idx.y, (int) idx.z, (int) idx.w));
          break;
        }
        case eVS_BoneWeights:
        {
          csVector4f w = *reinterpret_cast<csVector4f *>(&b8[i * attribute.Stride + attribute.Offset]);
          boneWeights.push_back(w);
          break;
        }
      }
    }
  }
  gl4Mesh->m_vertexBuffer->Unmap();

  gl4Mesh->m_indexBuffer->Map(&buffer, bufferSize);
  switch (gl4Mesh->m_indexDataType)
  {
    case eDT_UnsignedShort:
    {
      auto *s16 = reinterpret_cast<uint16_t *>(buffer);
      for (unsigned i = 0; i < gl4Mesh->GetNumberOfIndices(); i++)
      {
        uint16_t idx = *s16++;
        indices.push_back(idx + (uint32_t) vertexOffset);
      }
      break;
    }
    case eDT_UnsignedInt:
    {
      auto *s32 = reinterpret_cast<uint32_t *>(buffer);
      for (unsigned i = 0; i < gl4Mesh->GetNumberOfIndices(); i++)
      {
        uint32_t idx = *s32++;
        indices.push_back(idx + (uint32_t) vertexOffset);
      }
      break;

    }
    default:
      break;
  }
  gl4Mesh->m_indexBuffer->Unmap();


  if (!vertices2.empty())
  {
    m_generator->AddVertices(vertices2);
  }
  if (!vertices3.empty())
  {
    m_generator->AddVertices(vertices3);
  }
  if (!vertices4.empty())
  {
    m_generator->AddVertices(vertices4);
  }
  if (!normals.empty())
  {
    m_generator->AddNormals(normals);
  }
  if (!tangents.empty())
  {
    m_generator->AddTangents(tangents);
  }
  if (!colors.empty())
  {
    m_generator->AddColors(colors);
  }
  if (!uv02.empty())
  {
    m_generator->AddUV0(uv02);
  }
  if (!uv03.empty())
  {
    m_generator->AddUV0(uv03);
  }
  if (!uv1.empty())
  {
    m_generator->AddUV1(uv1);
  }
  if (!uv2.empty())
  {
    m_generator->AddUV2(uv2);
  }
  if (!uv3.empty())
  {
    m_generator->AddUV3(uv3);
  }
  if (!indices.empty())
  {
    m_generator->AddIndices(indices);
  }


}

iRenderMesh *csGL4RenderMeshBatchGenerator::Generate()
{
  return m_generator->Generate();
}


csGL4RenderMeshBatchGeneratorFactory::csGL4RenderMeshBatchGeneratorFactory(bool compatMode)
    : iRenderMeshBatchGeneratorFactory()
    , m_compatMode(compatMode)
{
  CS_CLASS_GEN_CONSTR;
}

iRenderMeshBatchGenerator *csGL4RenderMeshBatchGeneratorFactory::Create()
{
  return new csGL4RenderMeshBatchGenerator(m_compatMode);
}

}

