

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


GL4RenderMesh::GL4RenderMesh(UInt32 vao, GL4VertexBuffer* vb, GL4IndexBuffer* ib, ePrimitiveType primitiveType, eDataType indexType, Size count)
  : iRenderMesh()
  , m_vao(vao)
  , m_vertexBuffer(nullptr)
  , m_indexBuffer(nullptr)
  , m_primType(PrimitiveTypeMap[primitiveType])
  , m_indexType(DataTypeMap[indexType])
  , m_count(count)
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


void GL4RenderMesh::Render(iDevice* graphics, eRenderPass pass)
{
  glBindVertexArray(m_vao);
  glDrawElements(m_primType, m_count, m_indexType, reinterpret_cast<const void*>(0));
}











GL4RenderMeshGenerator::GL4RenderMeshGenerator()
  : iRenderMeshGenerator()
{
  SPC_CLASS_GEN_CONSTR;
}

GL4RenderMeshGenerator::~GL4RenderMeshGenerator()
{

}


void GL4RenderMeshGenerator::SetVertices(const std::vector<Vector3f>& vertices)
{
  m_vertices = vertices;
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
  m_uv0 = uv;
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
  if (m_indices.size() == 0 || m_vertices.size() == 0)
  {
    return nullptr;
  }

  std::vector<VertexDeclaration::Attribute> attributes;
  UInt16 offset = 0;
  attributes.push_back(VertexDeclaration::Attribute(0, eVS_Vertices, 3, eDT_Float, 0, 0));
  offset += 3 * sizeof(float);
  UInt16 count = 3;
  if (m_normals.size() != 0)
  {
    if (m_normals.size() != m_vertices.size())
    {
      return nullptr;
    }
    attributes.push_back(VertexDeclaration::Attribute(0, eVS_Normals, 3, eDT_Float, 0, offset));
    count += 3;
    offset += 3 * sizeof(float);
  }

  if (m_tangents.size() != 0)
  {
    if (m_tangents.size() != m_vertices.size())
    {
      return nullptr;
    }
    attributes.push_back(VertexDeclaration::Attribute(0, eVS_Tangents, 3, eDT_Float, 0, offset));
    count += 3;
    offset += 3 * sizeof(float);
  }
  if (m_uv0.size() != 0)
  {
    if (m_uv0.size() != m_vertices.size())
    {
      return nullptr;
    }
    attributes.push_back(VertexDeclaration::Attribute(0, eVS_UV0, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (m_uv1.size() != 0)
  {
    if (m_uv1.size() != m_vertices.size())
    {
      return nullptr;
    }
    attributes.push_back(VertexDeclaration::Attribute(0, eVS_UV1, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (m_uv2.size() != 0)
  {
    if (m_uv2.size() != m_vertices.size())
    {
      return nullptr;
    }
    attributes.push_back(VertexDeclaration::Attribute(0, eVS_UV2, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (m_uv3.size() != 0)
  {
    if (m_uv3.size() != m_vertices.size())
    {
      return nullptr;
    }
    attributes.push_back(VertexDeclaration::Attribute(0, eVS_UV3, 2, eDT_Float, 0, offset));
    count += 2;
    offset += 2 * sizeof(float);
  }
  if (m_colors.size() != 0)
  {
    if (m_colors.size() != m_vertices.size())
    {
      return nullptr;
    }
    attributes.push_back(VertexDeclaration::Attribute(0, eVS_Colors, 4, eDT_Float, 0, offset));
    count += 4;
    offset += 4 * sizeof(float);
  }
  for (VertexDeclaration::Attribute& attribute : attributes)
  {
    attribute.Stride = offset;
  }

  VertexDeclaration vd (attributes);
  float* vBuffer = new float[count * m_vertices.size()];

  for (Size i = 0, c = 0, in = m_vertices.size(); i < in; ++i)
  {
    {
      Vector3f& v = m_vertices[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (m_normals.size() != 0)
    {
      Vector3f& v = m_normals[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (m_tangents.size() != 0)
    {
      Vector3f& v = m_tangents[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
      vBuffer[c++] = v.z;
    }
    if (m_uv0.size() != 0)
    {
      Vector2f& v = m_uv0[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (m_uv1.size() != 0)
    {
      Vector2f& v = m_uv1[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (m_uv2.size() != 0)
    {
      Vector2f& v = m_uv2[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (m_uv3.size() != 0)
    {
      Vector2f& v = m_uv3[i];
      vBuffer[c++] = v.x;
      vBuffer[c++] = v.y;
    }
    if (m_colors.size() != 0)
    {
      Color4f& v = m_colors[i];
      vBuffer[c++] = v.r;
      vBuffer[c++] = v.g;
      vBuffer[c++] = v.b;
      vBuffer[c++] = v.a;
    }
  }

  GL4VertexBuffer* vb = new GL4VertexBuffer();
  vb->Bind();
  vb->CreateForRendering(m_vertices.size() * offset, eBU_Static);
  vb->Copy(vBuffer, m_vertices.size() * offset);
  delete[] vBuffer;

  GL4IndexBuffer* ib = new GL4IndexBuffer();
  eDataType indexType;
  if (m_vertices.size() > 65536)
  {
    ib->Bind();
    ib->CreateForRendering(m_indices.size() * sizeof(UInt32), eBU_Static);
    ib->Copy(&m_indices[0], m_indices.size() * sizeof(UInt32));
    indexType = eDT_UnsignedInt;
  }
  else
  {
    UInt16* iBuffer = new UInt16[m_indices.size()];
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
  const std::vector<VertexDeclaration::Attribute> & vdAttributes = vd.GetAttributes(0);
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

  GL4RenderMesh* mesh = new GL4RenderMesh(
    vao,
    vb,
    ib,
    ePT_Triangles,
    indexType,
    m_indices.size()
  );

  ib->Release();
  vb->Release();
  return mesh;
}


GL4RenderMeshGeneratorFactory::GL4RenderMeshGeneratorFactory()
  : iRenderMeshGeneratorFactory()
{
  SPC_CLASS_GEN_CONSTR;
}

GL4RenderMeshGeneratorFactory::~GL4RenderMeshGeneratorFactory()
{

}

iRenderMeshGenerator* GL4RenderMeshGeneratorFactory::Create()
{
  return new GL4RenderMeshGenerator();
}


}

