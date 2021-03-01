

#include <spcOpenGL/gl4/gl4rendermesh.hh>
#include <spcOpenGL/gl4/gl4vertexbuffer.hh>
#include <spcOpenGL/gl4/gl4indexbuffer.hh>
#include <spcCore/graphics/vertexdeclaration.hh>
#include <gl/glew.h>

namespace spc::opengl
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


GL4RenderMesh::GL4RenderMesh(UInt32 vao,
                             GL4VertexBuffer* vb,
                             GL4IndexBuffer* ib,
                             ePrimitiveType primitiveType,
                             eDataType indexType,
                             Size count,
                             const BoundingBox& boundingBox)
        : iRenderMesh()
        , m_vao(vao)
        , m_vertexBuffer(nullptr)
        , m_indexBuffer(nullptr)
        , m_primType(PrimitiveTypeMap[primitiveType])
        , m_indexType(DataTypeMap[indexType])
        , m_count(count)
        , m_boundingBox(boundingBox)
{
  SPC_CLASS_GEN_CONSTR;
  SPC_SET(m_vertexBuffer, vb);
  SPC_SET(m_indexBuffer, ib);

}

GL4RenderMesh::~GL4RenderMesh()
{
  if (m_vao != 0)
  {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }
  SPC_RELEASE(m_vertexBuffer);
  m_vertexBuffer = nullptr;

  SPC_RELEASE(m_indexBuffer);
  m_indexBuffer = nullptr;
}

const BoundingBox& GL4RenderMesh::GetBoundingBox() const
{
  return m_boundingBox;
}

void GL4RenderMesh::Render(iDevice* graphics, eRenderPass pass)
{
  glBindVertexArray(m_vao);
  glDrawElements(m_primType, (GLsizei) m_count, m_indexType, nullptr);
}


GL4RenderMeshGenerator::GL4RenderMeshGenerator()
        :
        iRenderMeshGenerator()
{
  SPC_CLASS_GEN_CONSTR;
}

GL4RenderMeshGenerator::~GL4RenderMeshGenerator() = default;


void GL4RenderMeshGenerator::SetVertices(const std::vector<Vector2f>& vertices)
{
  m_vertices2= vertices;
}

void GL4RenderMeshGenerator::SetVertices(const std::vector<Vector3f>& vertices)
{
  m_vertices3 = vertices;
}

void GL4RenderMeshGenerator::SetVertices(const std::vector<Vector4f>& vertices)
{
  m_vertices4 = vertices;
}


void GL4RenderMeshGenerator::SetNormals(const std::vector<Vector3f>& normals)
{
  m_normals = normals;
}

void GL4RenderMeshGenerator::SetColors(const std::vector<Color4f>& colors)
{
  m_colors = colors;
}

void GL4RenderMeshGenerator::SetTangents(const std::vector<Vector3f>& tangents)
{
  m_tangents = tangents;
}

void GL4RenderMeshGenerator::SetUV0(const std::vector<Vector2f>& uv)
{
  m_uv02 = uv;
}

void GL4RenderMeshGenerator::SetUV0(const std::vector<Vector3f>& uv)
{
  m_uv03 = uv;
}

void GL4RenderMeshGenerator::SetUV1(const std::vector<Vector2f>& uv)
{
  m_uv1 = uv;
}

void GL4RenderMeshGenerator::SetUV2(const std::vector<Vector2f>& uv)
{
  m_uv2 = uv;
}

void GL4RenderMeshGenerator::SetUV3(const std::vector<Vector2f>& uv)
{
  m_uv3 = uv;
}

void GL4RenderMeshGenerator::SetIndices(const std::vector<UInt32>& indices)
{
  m_indices = indices;
}

iRenderMesh* GL4RenderMeshGenerator::Generate()
{
  glBindVertexArray(0);
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

  std::vector<VertexDeclaration::Attribute> attributes;
  UInt16 offset = 0;
  UInt16 count = 0;
  Size vertexCount = 0;
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
  for (VertexDeclaration::Attribute& attribute : attributes)
  {
    attribute.Stride = offset;
  }

  BoundingBox bbox;
  bbox.Clear();
  VertexDeclaration vd(attributes);
  auto vBuffer = new float[count * vertexCount];

  for (Size i = 0, c = 0; i < vertexCount; ++i)
  {
    if (!m_vertices2.empty())
    {
      Vector2f& v = m_vertices2[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      bbox.Add(Vector3f(v.x, v.y, 0.0f));
    }
    else if (!m_vertices3.empty())
    {
      Vector3f& v = m_vertices3[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      bbox.Add(v);
    }
    else if (!m_vertices4.empty())
    {
      Vector4f& v = m_vertices4[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
      vBuffer[c++] = v.w;
      bbox.Add(v.AsVector3f());
    }

    if (!m_normals.empty())
    {
      Vector3f& v = m_normals[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_tangents.empty())
    {
      Vector3f& v = m_tangents[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_uv02.empty())
    {
      Vector2f& v = m_uv02[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    else if (!m_uv03.empty())
    {
      Vector3f& v = m_uv03[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (!m_uv1.empty())
    {
      Vector2f& v = m_uv1[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_uv2.empty())
    {
      Vector2f& v = m_uv2[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_uv3.empty())
    {
      Vector2f& v = m_uv3[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (!m_colors.empty())
    {
      Color4f& v = m_colors[i];
      vBuffer[c++] = v.r;
      vBuffer[c++] = v.g;
      vBuffer[c++] = v.b;
      vBuffer[c++] = v.a;
    }
  }
  bbox.Finish();

  auto vb = new GL4VertexBuffer();
  vb->Bind();
  vb->CreateForRendering(vertexCount* offset, eBU_Static);
  vb->Copy(vBuffer, vertexCount* offset);
  delete[] vBuffer;

  auto ib = new GL4IndexBuffer();
  eDataType indexType;
  if (vertexCount >= 65536 || true)
  {
    ib->Bind();
    ib->CreateForRendering(m_indices.size() * sizeof(UInt32), eBU_Static);
    ib->Copy(&m_indices[0], m_indices.size() * sizeof(UInt32));
    indexType = eDT_UnsignedInt;
  }
  else
  {
    auto iBuffer = new UInt16[m_indices.size()];
    for (Size i = 0, in = m_indices.size(); i < in; ++i)
    {
      iBuffer[i] = static_cast<UInt16>(m_indices[i]);
    }
    ib->Bind();
    ib->CreateForRendering(m_indices.size() * sizeof(UInt16), eBU_Static);
    ib->Copy(iBuffer, m_indices.size() * sizeof(UInt16));
    delete[] iBuffer;

    indexType = eDT_UnsignedShort;
  }

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  ib->Bind();
  vb->Bind();
  const std::vector<VertexDeclaration::Attribute>& vdAttributes = vd.GetAttributes(0);
  for (const VertexDeclaration::Attribute& attribute : vdAttributes)
  {
    glVertexAttribPointer(
            attribute.Location,
            attribute.Size,
            DataTypeMap[attribute.Type],
            false,
            attribute.Stride,
            reinterpret_cast<const void*>(attribute.Offset)
    );
    glEnableVertexAttribArray(attribute.Location);
  }
  glBindVertexArray(0);

  auto mesh = new GL4RenderMesh(
          vao,
          vb,
          ib,
          ePT_Triangles,
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
  SPC_CLASS_GEN_CONSTR;
}

GL4RenderMeshGeneratorFactory::~GL4RenderMeshGeneratorFactory() = default;

iRenderMeshGenerator* GL4RenderMeshGeneratorFactory::Create()
{
  return new GL4RenderMeshGenerator();
}


}

