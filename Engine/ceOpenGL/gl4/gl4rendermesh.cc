

#include <ceOpenGL/gl4/gl4rendermesh.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4vertexbuffer.hh>
#include <ceOpenGL/gl4/gl4indexbuffer.hh>
#include <ceOpenGL/glerror.hh>
#include <ceCore/graphics/vertexdeclaration.hh>
#include <gl/glew.h>


namespace ce::opengl
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


GL4RenderMesh::GL4RenderMesh(uint32_t vao,
                             const VertexDeclaration &vd,
                             Size vertexCount,
                             GL4VertexBuffer *vb,
                             GL4IndexBuffer *ib,
                             ePrimitiveType primitiveType,
                             eDataType indexType,
                             Size count,
                             const BoundingBox &boundingBox)
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
  CE_CLASS_GEN_CONSTR;
  CE_SET(m_vertexBuffer, vb);
  CE_SET(m_indexBuffer, ib);

}

GL4RenderMesh::~GL4RenderMesh()
{
  if (m_vao != 0)
  {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }
  CE_RELEASE(m_vertexBuffer);
  m_vertexBuffer = nullptr;

  CE_RELEASE(m_indexBuffer);
  m_indexBuffer = nullptr;
}

Size GL4RenderMesh::GetNumberOfIndices() const
{
  return m_count;
}

Size GL4RenderMesh::GetNumberOfVertices() const
{
  return m_vertexCount;
}

const BoundingBox &GL4RenderMesh::GetBoundingBox() const
{
  return m_boundingBox;
}

const VertexDeclaration &GL4RenderMesh::GetVertexDeclaration() const
{
  return m_vertexDeclaration;
}

void GL4RenderMesh::Render(iDevice *graphics, eRenderPass pass)
{
  CE_GL_ERROR();
  glBindVertexArray(m_vao);
  CE_GL_ERROR();
  glDrawElements(m_primType, (GLsizei) m_count, m_indexType, nullptr);
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    printf ("Error\n");
  }
  CE_GL_ERROR();
//  glBindVertexArray(0);
  CE_GL_ERROR();

#if _DEBUG
  auto gl4Device = graphics->Query<GL4Device>();
  gl4Device->IncDrawCalls();
  gl4Device->IncTriangles(GetNumberOfTriangles());
#endif
}

#if _DEBUG
Size GL4RenderMesh::GetNumberOfTriangles() const
{
  return m_count / 3;
}
#endif

GL4RenderMeshGenerator::GL4RenderMeshGenerator()
    : iRenderMeshGenerator()
    , m_primitiveType(ePT_Triangles)
{
  CE_CLASS_GEN_CONSTR;
}

GL4RenderMeshGenerator::~GL4RenderMeshGenerator() = default;


void GL4RenderMeshGenerator::SetPrimitiveType(ce::ePrimitiveType primitiveType)
{
  m_primitiveType = primitiveType;
}

void GL4RenderMeshGenerator::SetVertices(const std::vector<Vector2f> &vertices)
{
  m_vertices2 = vertices;
}

void GL4RenderMeshGenerator::SetVertices(const std::vector<Vector3f> &vertices)
{
  m_vertices3 = vertices;
}

void GL4RenderMeshGenerator::SetVertices(const std::vector<Vector4f> &vertices)
{
  m_vertices4 = vertices;
}


void GL4RenderMeshGenerator::SetNormals(const std::vector<Vector3f> &normals)
{
  m_normals = normals;
}

void GL4RenderMeshGenerator::SetColors(const std::vector<Color4f> &colors)
{
  m_colors = colors;
}

void GL4RenderMeshGenerator::SetTangents(const std::vector<Vector3f> &tangents)
{
  m_tangents = tangents;
}

void GL4RenderMeshGenerator::SetUV0(const std::vector<Vector2f> &uv)
{
  m_uv02 = uv;
}

void GL4RenderMeshGenerator::SetUV0(const std::vector<Vector3f> &uv)
{
  m_uv03 = uv;
}

void GL4RenderMeshGenerator::SetUV1(const std::vector<Vector2f> &uv)
{
  m_uv1 = uv;
}

void GL4RenderMeshGenerator::SetUV2(const std::vector<Vector2f> &uv)
{
  m_uv2 = uv;
}

void GL4RenderMeshGenerator::SetUV3(const std::vector<Vector2f> &uv)
{
  m_uv3 = uv;
}

void GL4RenderMeshGenerator::SetBoneIndices(const std::vector<Vector4i> &boneIndices)
{
  m_boneIndices = boneIndices;
}

void GL4RenderMeshGenerator::SetBoneWeights(const std::vector<Vector4f> &boneWeights)
{
  m_boneWeights = boneWeights;
}

void GL4RenderMeshGenerator::SetIndices(const std::vector<uint32_t> &indices)
{
  m_indices = indices;
}

void GL4RenderMeshGenerator::AddVertices(const std::vector<Vector2f> &vertices)
{
  m_vertices2.insert(m_vertices2.end(), vertices.begin(), vertices.end());
}

void GL4RenderMeshGenerator::AddVertices(const std::vector<Vector3f> &vertices)
{
  m_vertices3.insert(m_vertices3.end(), vertices.begin(), vertices.end());
}

void GL4RenderMeshGenerator::AddVertices(const std::vector<Vector4f> &vertices)
{
  m_vertices4.insert(m_vertices4.end(), vertices.begin(), vertices.end());
}


void GL4RenderMeshGenerator::AddNormals(const std::vector<Vector3f> &normals)
{
  m_normals.insert(m_normals.end(), normals.begin(), normals.end());
}

void GL4RenderMeshGenerator::AddColors(const std::vector<Color4f> &colors)
{
  m_colors.insert(m_colors.end(), colors.begin(), colors.end());
}

void GL4RenderMeshGenerator::AddTangents(const std::vector<Vector3f> &tangents)
{
  m_tangents.insert(m_tangents.end(), tangents.begin(), tangents.end());
}

void GL4RenderMeshGenerator::AddUV0(const std::vector<Vector2f> &uv)
{
  m_uv02.insert(m_uv02.end(), uv.begin(), uv.end());
}

void GL4RenderMeshGenerator::AddUV0(const std::vector<Vector3f> &uv)
{
  m_uv03.insert(m_uv03.end(), uv.begin(), uv.end());
}

void GL4RenderMeshGenerator::AddUV1(const std::vector<Vector2f> &uv)
{
  m_uv1.insert(m_uv1.end(), uv.begin(), uv.end());
}

void GL4RenderMeshGenerator::AddUV2(const std::vector<Vector2f> &uv)
{
  m_uv2.insert(m_uv2.end(), uv.begin(), uv.end());
}

void GL4RenderMeshGenerator::AddUV3(const std::vector<Vector2f> &uv)
{
  m_uv3.insert(m_uv3.end(), uv.begin(), uv.end());
}

void GL4RenderMeshGenerator::AddBoneIndices(const std::vector<Vector4i> &boneIndices)
{
  m_boneIndices.insert(m_boneIndices.end(), boneIndices.begin(), boneIndices.end());
}

void GL4RenderMeshGenerator::AddBoneWeights(const std::vector<Vector4f> &boneWeights)
{
  m_boneWeights.insert(m_boneWeights.end(), boneWeights.begin(), boneWeights.end());
}

void GL4RenderMeshGenerator::AddIndices(const std::vector<uint32_t> &indices)
{
  m_indices.insert(m_indices.end(), indices.begin(), indices.end());
}


size_t GL4RenderMeshGenerator::GetNumberOfVertices() const
{
  return std::max(m_vertices2.size(), std::max(m_vertices3.size(), m_vertices4.size()));
}

iRenderMesh *GL4RenderMeshGenerator::Generate()
{
  glBindVertexArray(0);
  int numVertexDefs = 0;
  int numUV0Defs    = 0;
  numVertexDefs += m_vertices2.empty() ? 0 : 1;
  numVertexDefs += m_vertices3.empty() ? 0 : 1;
  numVertexDefs += m_vertices4.empty() ? 0 : 1;
  numUV0Defs += m_uv02.empty() ? 0 : 1;
  numUV0Defs += m_uv03.empty() ? 0 : 1;
  if (m_indices.empty() || numVertexDefs != 1 || numUV0Defs > 1)
  {
    return nullptr;
  }

  std::vector<VertexDeclaration::Attribute> attributes;
  uint16_t                                  offset      = 0;
  uint16_t                                  count       = 0;
  Size                                      vertexCount = 0;
  if (!m_vertices2.empty())
  {
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Vertices, 2, eDT_Float, 0, 0));
    offset += 2 * sizeof(float);
    count += 2;
    vertexCount = m_vertices2.size();
  }
  else if (!m_vertices3.empty())
  {
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Vertices, 3, eDT_Float, 0, 0));
    offset += 3 * sizeof(float);
    count += 3;
    vertexCount = m_vertices3.size();
  }
  else if (!m_vertices4.empty())
  {
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Vertices, 4, eDT_Float, 0, 0));
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
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Normals, 3, eDT_Float, 0, offset));
    count += 3;
    offset += 3 * sizeof(float);
  }

  if (!m_tangents.empty())
  {
    if (m_tangents.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Tangents, 3, eDT_Float, 0, offset));
    count += 3;
    offset += 3 * sizeof(float);
  }
  if (!m_uv02.empty())
  {
    if (m_uv02.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_UV0, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  else if (!m_uv03.empty())
  {
    if (m_uv03.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_UV0, 3, eDT_Float, 0, offset));
    count += 3;
    offset += 3 * sizeof(float);
  }
  if (!m_uv1.empty())
  {
    if (m_uv1.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_UV1, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (!m_uv2.empty())
  {
    if (m_uv2.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_UV2, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (!m_uv3.empty())
  {
    if (m_uv3.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_UV3, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (!m_colors.empty())
  {
    if (m_colors.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Colors, 4, eDT_Float, 0, offset));
    count += 4;
    offset += 4 * sizeof(float);
  }
  if (!m_boneIndices.empty())
  {
    if (m_boneIndices.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_BoneIndices, 4, eDT_Float, 0, offset));
    count += 4;
    offset += 4 * sizeof(float);
  }
  if (!m_boneWeights.empty())
  {
    if (m_boneWeights.size() != vertexCount)
    {
      return nullptr;
    }
    attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_BoneWeights, 4, eDT_Float, 0, offset));
    count += 4;
    offset += 4 * sizeof(float);
  }

  for (VertexDeclaration::Attribute &attribute: attributes)
  {
    attribute.Stride = offset;
  }

  BoundingBox bbox;
  bbox.Clear();
  VertexDeclaration vd(attributes);
  auto              vBuffer = new float[count * vertexCount];

  for (Size i = 0, c = 0; i < vertexCount; ++i)
  {
    if (!m_vertices2.empty())
    {
      Vector2f &v = m_vertices2[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      bbox.Add(Vector3f(v.x, v.y, 0.0f));
    }
    else if (!m_vertices3.empty())
    {
      Vector3f &v = m_vertices3[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      bbox.Add(v);
    }
    else if (!m_vertices4.empty())
    {
      Vector4f &v = m_vertices4[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      vBuffer[c++] = v.w;
      bbox.Add((Vector3f) v);
    }

    if (!m_normals.empty())
    {
      Vector3f &v = m_normals[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_tangents.empty())
    {
      Vector3f &v  = m_tangents[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_uv02.empty())
    {
      Vector2f &v  = m_uv02[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    else if (!m_uv03.empty())
    {
      Vector3f &v = m_uv03[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_uv1.empty())
    {
      Vector2f &v  = m_uv1[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_uv2.empty())
    {
      Vector2f &v  = m_uv2[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_uv3.empty())
    {
      Vector2f &v  = m_uv3[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_colors.empty())
    {
      Color4f &v   = m_colors[i];
      vBuffer[c++] = v.r;
      vBuffer[c++] = v.g;
      vBuffer[c++] = v.b;
      vBuffer[c++] = v.a;
    }
    if (!m_boneIndices.empty())
    {
      Vector4i &v  = m_boneIndices[i];
      vBuffer[c++] = (float) v.x + 0.25f;
      vBuffer[c++] = (float) v.y + 0.25f;
      vBuffer[c++] = (float) v.z + 0.25f;
      vBuffer[c++] = (float) v.w + 0.25f;
    }
    if (!m_boneWeights.empty())
    {
      Vector4f &v  = m_boneWeights[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      vBuffer[c++] = v.w;
    }
  }
  bbox.Finish();

  auto vb = new GL4VertexBuffer();
  vb->Bind();
  vb->CreateForRendering(vertexCount * offset, eBU_Static);
  vb->Copy(vBuffer, vertexCount * offset);
  delete[] vBuffer;

  auto      ib = new GL4IndexBuffer();
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
    auto      iBuffer = new uint16_t[m_indices.size()];
    for (Size i       = 0, in = m_indices.size(); i < in; ++i)
    {
      iBuffer[i] = static_cast<uint16_t>(m_indices[i]);
    }
    ib->Bind();
    ib->CreateForRendering(m_indices.size() * sizeof(uint16_t), eBU_Static);
    ib->Copy(iBuffer, m_indices.size() * sizeof(uint16_t));
    delete[] iBuffer;

    indexType = eDT_UnsignedShort;
  }

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  ib->Bind();
  vb->Bind();
  const std::vector<VertexDeclaration::Attribute> &vdAttributes = vd.GetAttributes(0);
  for (const VertexDeclaration::Attribute         &attribute: vdAttributes)
  {
    glVertexAttribPointer(
        attribute.Location,
        attribute.Size,
        DataTypeMap[attribute.Type],
        false,
        attribute.Stride,
        reinterpret_cast<const void *>(attribute.Offset)
    );
    glEnableVertexAttribArray(attribute.Location);
  }
  glBindVertexArray(0);

  auto mesh = new GL4RenderMesh(
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


GL4RenderMeshGeneratorFactory::GL4RenderMeshGeneratorFactory()
    :
    iRenderMeshGeneratorFactory()
{
  CE_CLASS_GEN_CONSTR;
}

iRenderMeshGenerator *GL4RenderMeshGeneratorFactory::Create()
{
  return new GL4RenderMeshGenerator();
}


void GL4RenderMeshBatchGenerator::Add(const iRenderMesh *mesh, const Matrix4f &matrix)
{
  auto                  gl4Mesh = static_cast<const GL4RenderMesh *>(mesh);
  std::vector<Vector2f> vertices2;
  std::vector<Vector3f> vertices3;
  std::vector<Vector4f> vertices4;
  std::vector<Vector3f> normals;
  std::vector<Color4f>  colors;
  std::vector<Vector3f> tangents;
  std::vector<Vector2f> uv02;
  std::vector<Vector3f> uv03;
  std::vector<Vector2f> uv1;
  std::vector<Vector2f> uv2;
  std::vector<Vector2f> uv3;
  std::vector<Vector4i> boneIndices;
  std::vector<Vector4f> boneWeights;
  std::vector<uint32_t> indices;

  size_t vertexOffset = m_generator.GetNumberOfVertices();
  auto   rotMat       = (Matrix3f) matrix;
  size_t numVertices  = gl4Mesh->GetNumberOfVertices();
  void *buffer = nullptr;
  Size bufferSize = 0;
  gl4Mesh->m_vertexBuffer->Map(&buffer, bufferSize);
  auto            *b8 = reinterpret_cast<uint8_t *>(buffer);
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
              Vector2f v2 = *reinterpret_cast<Vector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
              vertices2.push_back((Vector2f) (matrix * Vector4f(v2, 0, 1)));
              break;
            }
            case 3:
            {
              Vector3f v3 = *reinterpret_cast<Vector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
              vertices3.push_back((Vector3f) (matrix * Vector4f(v3, 1)));
              break;
            }
            case 4:
            {
              Vector4f v4 = *reinterpret_cast<Vector4f *>(&b8[i * attribute.Stride + attribute.Offset]);
              vertices4.push_back(matrix * v4);
              break;
            }
          }
          break;
        case eVS_Normals:
        {
          Vector3f v3 = *reinterpret_cast<Vector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
          normals.push_back(rotMat * v3);
          break;
        }
        case eVS_Tangents:
        {
          Vector3f v3 = *reinterpret_cast<Vector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
          tangents.push_back(rotMat * v3);
          break;
        }
        case eVS_Colors:
        {
          Color4f c = *reinterpret_cast<Color4f *>(&b8[i * attribute.Stride + attribute.Offset]);
          colors.push_back(c);
          break;
        }
        case eVS_UV0:
          switch (attribute.Size)
          {
            case 2:
            {
              Vector2f v2 = *reinterpret_cast<Vector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
              uv02.push_back(v2);
              break;
            }
            case 3:
            {
              Vector3f v3 = *reinterpret_cast<Vector3f *>(&b8[i * attribute.Stride + attribute.Offset]);
              uv03.push_back(v3);
              break;
            }
          }
          break;
        case eVS_UV1:
        {
          Vector2f v2 = *reinterpret_cast<Vector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
          uv1.push_back(v2);
          break;
        }
        case eVS_UV2:
        {
          Vector2f v2 = *reinterpret_cast<Vector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
          uv2.push_back(v2);
          break;
        }
        case eVS_UV3:
        {
          Vector2f v2 = *reinterpret_cast<Vector2f *>(&b8[i * attribute.Stride + attribute.Offset]);
          uv3.push_back(v2);
          break;
        }
        case eVS_BoneIndices:
        {
          Vector4f idx = *reinterpret_cast<Vector4f *>(&b8[i * attribute.Stride + attribute.Offset]);
          boneIndices.emplace_back(Vector4i((int) idx.x, (int) idx.y, (int) idx.z, (int) idx.w));
          break;
        }
        case eVS_BoneWeights:
        {
          Vector4f w = *reinterpret_cast<Vector4f *>(&b8[i * attribute.Stride + attribute.Offset]);
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
      auto          *s16 = reinterpret_cast<uint16_t *>(buffer);
      for (unsigned i    = 0; i < gl4Mesh->GetNumberOfIndices(); i++)
      {
        uint16_t idx = *s16++;
        indices.push_back(idx + (uint32_t) vertexOffset);
      }
      break;
    }
    case eDT_UnsignedInt:
    {
      auto          *s32 = reinterpret_cast<uint32_t *>(buffer);
      for (unsigned i    = 0; i < gl4Mesh->GetNumberOfIndices(); i++)
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
    m_generator.AddVertices(vertices2);
  }
  if (!vertices3.empty())
  {
    m_generator.AddVertices(vertices3);
  }
  if (!vertices4.empty())
  {
    m_generator.AddVertices(vertices4);
  }
  if (!normals.empty())
  {
    m_generator.AddNormals(normals);
  }
  if (!tangents.empty())
  {
    m_generator.AddTangents(tangents);
  }
  if (!colors.empty())
  {
    m_generator.AddColors(colors);
  }
  if (!uv02.empty())
  {
    m_generator.AddUV0(uv02);
  }
  if (!uv03.empty())
  {
    m_generator.AddUV0(uv03);
  }
  if (!uv1.empty())
  {
    m_generator.AddUV1(uv1);
  }
  if (!uv2.empty())
  {
    m_generator.AddUV2(uv2);
  }
  if (!uv3.empty())
  {
    m_generator.AddUV3(uv3);
  }
  if (!indices.empty())
  {
    m_generator.AddIndices(indices);
  }


}

iRenderMesh *GL4RenderMeshBatchGenerator::Generate()
{
  return m_generator.Generate();
}


GL4RenderMeshBatchGeneratorFactory::GL4RenderMeshBatchGeneratorFactory()
    :
    iRenderMeshBatchGeneratorFactory()
{
  CE_CLASS_GEN_CONSTR;
}

iRenderMeshBatchGenerator *GL4RenderMeshBatchGeneratorFactory::Create()
{
  return new GL4RenderMeshBatchGenerator();
}

}

