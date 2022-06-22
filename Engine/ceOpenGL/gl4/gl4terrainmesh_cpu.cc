
#include <ceOpenGL/gl4/gl4terrainmesh_cpu.hh>

#include <ceOpenGL/gl4/gl4indexbuffer.hh>
#include <ceOpenGL/gl4/gl4vertexbuffer.hh>
#include <ceOpenGL/glerror.hh>
#include <gl/glew.h>


namespace ce::opengl
{


GL4TerrainMeshCPU::GL4TerrainMeshCPU(uint32_t vao,
                                     const VertexDeclaration& vd,
                                     iVertexBuffer* vb,
                                     iIndexBuffer* ib,
                                     const BoundingBox& bbox,
                                     const std::vector<Patch>& patches,
                                     eTerrainSize terrainSize,
                                     eTerrainSize patchSize)
  : iTerrainMesh(), m_vao(vao), m_vd(vd), m_vb(nullptr), m_ib(nullptr), m_bbox(bbox), m_patches(patches),
    m_terrainSize(terrainSize), m_patchSize(patchSize)
{
  CE_CLASS_GEN_CONSTR;
  CE_SET(m_vb, vb);
  CE_SET(m_ib, ib);
}

const BoundingBox& GL4TerrainMeshCPU::GetBoundingBox() const
{
  return m_bbox;
}

const VertexDeclaration& GL4TerrainMeshCPU::GetVertexDeclaration() const
{
  return m_vd;
}

void GL4TerrainMeshCPU::SetReferencePoint(const Vector3f& refPoint)
{
  m_referencePoint = refPoint;
}

void GL4TerrainMeshCPU::Render(iDevice* graphics, eRenderPass pass)
{
  CE_GL_ERROR();
  Update();
  CE_GL_ERROR();
  glBindVertexArray(m_vao);
  CE_GL_ERROR();

  CE_GL_ERROR();
  glBindVertexArray(0);
  CE_GL_ERROR();

#if _DEBUG
  /*
  auto gl4Device = graphics->Query<GL4Device>();
  gl4Device->IncDrawCalls();
  gl4Device->IncTriangles(GetNumberOfTriangles());
   */
#endif

}

bool GL4TerrainMeshCPU::Line::UpdateStepSize(const Vector3f& refPoint)
{
  // revalidate the real step size here... for now just set to maximum
  if (absStepSize != useStepSize)
  {
    useStepSize = absStepSize;
    return true;
  }
  return false;
}

bool GL4TerrainMeshCPU::Patch::UpdateIndices(const Vector3f& refPoint, eTerrainSize size)
{
  bool changed = lineTop.UpdateStepSize(refPoint);
  changed |= lineBottom.UpdateStepSize(refPoint);
  changed |= lineLeft.UpdateStepSize(refPoint);
  changed |= lineRight.UpdateStepSize(refPoint);
  if (!changed && buffer)
  {
    return false;
  }

  RegenerateIndices(size);
  return true;
}

void GL4TerrainMeshCPU::Patch::RegenerateIndices(eTerrainSize size)
{
  auto pSize = static_cast<size_t>(patchSize);
  auto tSize = static_cast<size_t>(size);
  if (!buffer)
  {
    buffer = new uint32_t[(pSize - 1) * (pSize - 1) * 6];
  }

  uint32_t* iptr = buffer;

  for (size_t i = 0; i < pSize - 1; ++i)
  {
    size_t iv0 = v00 + i * tSize;
    size_t iv1 = iv0 + tSize;

    for (size_t j = 0; j < pSize - 1; ++j)
    {
      size_t iv00 = iv0 + j;
      size_t iv01 = iv00 + 1;
      size_t iv10 = iv1 + j;
      size_t iv11 = iv10 + 1;

      *iptr++ = (uint32_t)iv00;
      *iptr++ = (uint32_t)iv01;
      *iptr++ = (uint32_t)iv11;
      *iptr++ = (uint32_t)iv00;
      *iptr++ = (uint32_t)iv11;
      *iptr++ = (uint32_t)iv10;
    }
  }

  bufferCount = (pSize-1) * (pSize-1) * 6;
}

void GL4TerrainMeshCPU::Update()
{
  if (UpdatePatches())
  {
    RebuildIndices();
  }
}

bool GL4TerrainMeshCPU::UpdatePatches()
{
  bool needUpdate = false;
  for (auto patch : m_patches)
  {
    needUpdate |= patch.UpdateIndices(m_referencePoint, m_terrainSize);
  }
  return needUpdate;
}

void GL4TerrainMeshCPU::RebuildIndices()
{
  m_ib->Bind();
  size_t offset = 0;
  for (auto patch : m_patches)
  {
    size_t patchSize = sizeof(uint32_t) * patch.bufferCount;
    m_ib->Copy(patch.buffer, patchSize, offset);
    offset += patchSize;
  }
}


GL4TerrainMeshGeneratorCPU::GL4TerrainMeshGeneratorCPU()
  : iTerrainMeshGenerator()
{
  CE_CLASS_GEN_CONSTR;
}

void GL4TerrainMeshGeneratorCPU::SetSize(eTerrainSize size)
{
  m_size = size;
}
void GL4TerrainMeshGeneratorCPU::SetPatchSize(eTerrainSize size)
{
  m_patchSize = size;
}
void GL4TerrainMeshGeneratorCPU::SetSize(const Vector3f& min, const Vector3f& max)
{
  m_min = min;
  m_max = max;
}
void GL4TerrainMeshGeneratorCPU::SetNormalizedHeightData(const std::vector<float>& heightData)
{
  m_heightData = heightData;

  float min = m_heightData[0];
  float max = m_heightData[0];

  for (float v : m_heightData)
  {
    min = min < v ? min : v;
    max = max > v ? max : v;
  }

  float d = max - min;
  if (d != 0.0f)
  {
    for (float& heightValue : m_heightData)
    {
      heightValue = (heightValue - min) / d;
    }
  }
}

void GL4TerrainMeshGeneratorCPU::GenerateVerticesUVs(std::vector<Vector3f>& vertices, std::vector<Vector2f>& uv)
{
  auto     size = static_cast<size_t>(m_size);
  Vector3f d    = m_max - m_min;

  for (size_t i = 0; i < size; i++)
  {
    float       nz = (float)i / (float)size;
    float       z  = m_min.z + nz * d.z;
    for (size_t j  = 0; j < size; j++)
    {
      float nx = (float)j / (float)size;
      float x  = m_min.x + nx * d.x;

      float ny = m_heightData[i * size + j];
      float y  = m_min.y + ny * d.y;

      vertices.emplace_back(x, y, z);
      uv.emplace_back(nx, nz);
    }
  }
}

void GL4TerrainMeshGeneratorCPU::GenerateNormals(std::vector<Vector3f>& vertices,
                                                 std::vector<Vector3f>& normals)
{
  auto size = static_cast<size_t>(m_size);

  for (size_t i = 0, idx = 0; i < size; i++)
  {
    bool        haveTop    = i != 0;
    bool        haveBottom = i != (size - 1);
    for (size_t j          = 0; j < size; j++, idx++)
    {
      bool haveLeft  = j != 0;
      bool haveRight = j != (size - 1);

      const Vector3f& v  = vertices[idx];
      const Vector3f& vl = haveLeft ? vertices[idx - 1] : vertices[idx];
      const Vector3f& vr = haveRight ? vertices[idx + 1] : vertices[idx];
      const Vector3f& vt = haveTop ? vertices[idx - size] : vertices[idx];
      const Vector3f& vb = haveBottom ? vertices[idx + size] : vertices[idx];

      const Vector3f dl = vl - v;
      const Vector3f dr = vr - v;
      const Vector3f dt = vt - v;
      const Vector3f db = vb - v;

      Vector3f n(0.0f, 0.0f, 0.0f);
      if (haveLeft && haveTop)
      {
        n += dl % dt;
      }
      if (haveTop && haveRight)
      {
        n += dt % dr;
      }
      if (haveRight && haveBottom)
      {
        n += dr % db;
      }
      if (haveBottom && haveLeft)
      {
        n += db % dl;
      }

      normals.push_back(n.Normalized());
    }
  }
}

void GL4TerrainMeshGeneratorCPU::EvalLine(std::vector<Vector3f>& vertices,
                                          size_t v0,
                                          size_t v1,
                                          size_t stepSize,
                                          GL4TerrainMeshCPU::Line& line)
{
  Vector3f    min  = vertices[v0];
  Vector3f    max  = vertices[v0];
  for (size_t i    = v0; i <= v1; i += stepSize)
  {
    Vector3f& v = vertices[i];
    min.x = min.x < v.x ? min.x : v.x;
    min.y = min.y < v.y ? min.y : v.y;
    min.z = min.z < v.z ? min.z : v.z;
    max.x = max.x > v.x ? max.x : v.x;
    max.y = max.y > v.y ? max.y : v.y;
    max.z = max.z > v.z ? max.z : v.z;
  }
  line.v0          = v0;
  line.v1          = v1;
  line.min         = min;
  line.max         = max;
  line.absStepSize = stepSize;
  line.useStepSize = stepSize;
}

void GL4TerrainMeshGeneratorCPU::GeneratePatches(std::vector<Vector3f>& vertices,
                                                 std::vector<GL4TerrainMeshCPU::Patch>& patches)
{
  auto   size       = static_cast<size_t>(m_patchSize);
  size_t numPatches = (static_cast<size_t>(m_size) - 1) / (static_cast<size_t>(m_patchSize) - 1);

  for (size_t i = 0, idx = 0; i < numPatches; ++i)
  {
    size_t v0 = i * size;
    size_t v1 = v0 + size;

    for (size_t j = 0; j < numPatches; ++j, ++idx)
    {
      size_t v00 = v0 + j;
      size_t v01 = v00 + 1;
      size_t v10 = v1 + j;
      size_t v11 = v10 + 1;

      GL4TerrainMeshCPU::Patch& patch = patches[idx];
      EvalLine(vertices, v00, v01, 1, patch.lineTop);
      EvalLine(vertices, v10, v11, 1, patch.lineBottom);
      EvalLine(vertices, v00, v10, size, patch.lineLeft);
      EvalLine(vertices, v01, v11, size, patch.lineRight);

      patch.v00 = v00;
      patch.v01 = v01;
      patch.v10 = v10;
      patch.v11 = v11;
    }
  }
}

iTerrainMesh* GL4TerrainMeshGeneratorCPU::Generate()
{
  auto   size        = static_cast<size_t>(m_size);
  size_t vertexCount = size * size;
  if (vertexCount > m_heightData.size())
  {
    return nullptr;
  }
  size_t numPatches = (static_cast<size_t>(m_size) - 1) / (static_cast<size_t>(m_patchSize) - 1);

  std::vector<Vector3f>                 vertices(vertexCount);
  std::vector<Vector3f>                 normals(vertexCount);
  std::vector<Vector2f>                 uvs(vertexCount);
  std::vector<GL4TerrainMeshCPU::Patch> patches(numPatches * numPatches);

  GenerateVerticesUVs(vertices, uvs);
  GenerateNormals(vertices, normals);
  GeneratePatches(vertices, patches);


  glBindVertexArray(0);

  uint16_t vertexSize = sizeof(float) * 8;


  // generate the vertex declaration
  std::vector<VertexDeclaration::Attribute> attributes;
  attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Vertices, 3, eDT_Float, vertexSize, 0));
  attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_Normals, 3, eDT_Float, vertexSize, sizeof(float) * 3));
  attributes.emplace_back(VertexDeclaration::Attribute(0, eVS_UV, 2, eDT_Float, vertexSize, sizeof(float) * 6));
  VertexDeclaration vd(attributes);

  // generate the bounding box
  BoundingBox bbox;
  bbox.Clear();
  for (auto& v : vertices)
  {
    bbox.Add(v);
  }
  bbox.Finish();

  // generate the vertices
  auto * vBuffer = new float[8 * vertexCount];
  float* vptr    = vBuffer;
  for (size_t i = 0; i < vertexCount; ++i, ++vptr)
  {
    const Vector3f& vertex = vertices[i];
    const Vector3f& normal = normals[i];
    const Vector2f& uv     = uvs[i];
    *vptr++ = vertex.x;
    *vptr++ = vertex.y;
    *vptr++ = vertex.z;
    *vptr++ = normal.x;
    *vptr++ = normal.y;
    *vptr++ = normal.z;
    *vptr++ = uv.x;
    *vptr++ = uv.y;
  }

  auto vb = new GL4VertexBuffer();
  vb->Bind();
  vb->CreateForRendering(vertexCount * sizeof(float) * 8, eBU_Static);
  vb->Copy(vBuffer, vertexCount * vertexSize);
  delete[] vBuffer;

  // initialize an empty index buffer
  auto ib = new GL4IndexBuffer;
  ib->Bind();
  ib->CreateForRendering((size - 1) * (size - 1) * 6 * 4);


  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


  ib->Bind();
  vb->Bind();

  for (auto& attribute : attributes)
  {
    glVertexAttribPointer(
      attribute.Location,
      attribute.Size,
      GL_FLOAT,
      false,
      attribute.Stride,
      reinterpret_cast<const void*>(attribute.Offset)
    );
    glEnableVertexAttribArray(attribute.Location);
  }
  glBindVertexArray(0);


  return new GL4TerrainMeshCPU(
    vao,
    vd,
    vb,
    ib,
    bbox,
    patches,
    m_size,
    m_patchSize);
}

}
