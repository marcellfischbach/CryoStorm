
#include <ceOpenGL/gl4/gl4terrainmesh_cpu.hh>

#include <ceOpenGL/gl4/gl4indexbuffer.hh>
#include <ceOpenGL/gl4/gl4vertexbuffer.hh>
#include <ceOpenGL/glerror.hh>

#include <ceOpenGL/gl4/gl4texture2d.hh>
#include <gl/glew.h>


namespace ce::opengl
{


GL4TerrainMeshCPU::GL4TerrainMeshCPU(uint32_t vao,
                                     const VertexDeclaration &vd,
                                     iVertexBuffer *vb,
                                     iIndexBuffer *ib,
                                     const BoundingBox &bbox,
                                     const std::vector<Patch> &patches,
                                     eTerrainSize terrainSize,
                                     eTerrainSize patchSize)
    : iTerrainMesh()
    , m_vao(vao)
    , m_vd(vd)
    , m_vb(nullptr)
    , m_ib(nullptr)
    , m_bbox(bbox)
    , m_patches(patches)
    , m_terrainSize(terrainSize)
    , m_patchSize(patchSize)
{
  CE_CLASS_GEN_CONSTR;
  CE_SET(m_vb, vb);
  CE_SET(m_ib, ib);
}

const BoundingBox &GL4TerrainMeshCPU::GetBoundingBox() const
{
  return m_bbox;
}

const VertexDeclaration &GL4TerrainMeshCPU::GetVertexDeclaration() const
{
  return m_vd;
}

void GL4TerrainMeshCPU::SetReferencePoint(const Vector3f &refPoint)
{
  m_referencePoint = refPoint;
}

void GL4TerrainMeshCPU::SetLayerMask(const TerrainLayerMask& layerMask)
{
  m_layerMask = layerMask;
  CE_ADDREF(m_layerMask.LayerTexture);
  CE_ADDREF(m_layerMask.MaskTexture);

  UpdateMaterial();
}

void GL4TerrainMeshCPU::AddLayer(const TerrainLayer& layer)
{
  m_layers.push_back(layer);
  CE_ADDREF(layer.DiffuseRoughness);
  CE_ADDREF(layer.Normal);

  UpdateMaterial ();
}

void GL4TerrainMeshCPU::UpdateMaterial()
{

}

void GL4TerrainMeshCPU::Render(iDevice *graphics, eRenderPass pass)
{
  CE_GL_ERROR();
  Update();
  CE_GL_ERROR();
  glBindVertexArray(m_vao);
  CE_GL_ERROR();

  glDrawElements(GL_TRIANGLES, (GLsizei) m_indexBufferSize, GL_UNSIGNED_INT, nullptr);

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

bool GL4TerrainMeshCPU::Line::UpdateStepSize(const Vector3f &refPoint, size_t override)
{
  Vector3f delta = center - refPoint;
  float distance = delta.Length();

  float minDistance = 5.0f;
  float maxDistance = 30.0f;

  float distFact = (distance - minDistance) / (maxDistance - minDistance);
  distFact = std::max(0.0f, std::min(distFact, 1.0f));

  size_t stepSize = override;
  size_t patchSize2 = (patchSize - 1) / 2;

  float dfact = 1.0 + distFact * (patchSize2-1);
  float log = log2(dfact);
  int logi = (int)log;

  stepSize = 1 << logi;
  /*
  if (stepSize < 1)
  {
    stepSize = 1;
  }
  if (stepSize > patchSize2)
  {
    stepSize = patchSize2;
  }
  */
  // revalidate the real step size here... for now just set to maximum
  if (absStepSize != stepSize)
  {
    useStepSize = stepSize;
    return true;
  }
  return false;
}

bool GL4TerrainMeshCPU::Patch::UpdateIndices(const Vector3f &refPoint, eTerrainSize size)
{
  bool changed = lineTop.UpdateStepSize(refPoint, 1);
  changed |= lineBottom.UpdateStepSize(refPoint, 1);
  changed |= lineLeft.UpdateStepSize(refPoint, 8);
  changed |= lineRight.UpdateStepSize(refPoint, 8);
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
  auto tSize = static_cast<int64_t>(size);

  if (!buffer)
  {
    buffer = new uint32_t[(pSize - 1) * (pSize - 1) * 6];
  }

  bufferCount = 0;

  bufferCount += GenerateGeneric(tSize, -1, lineTop.useStepSize, lineLeft.useStepSize, true, bufferCount);
  bufferCount += GenerateGeneric(tSize, 1, lineTop.useStepSize, lineRight.useStepSize, false, bufferCount);
  bufferCount += GenerateGeneric(-tSize, -1, lineBottom.useStepSize, lineLeft.useStepSize, false, bufferCount);
  bufferCount += GenerateGeneric(-tSize, 1, lineBottom.useStepSize, lineRight.useStepSize, true, bufferCount);
  bufferCount += GenerateGeneric( -1, tSize, lineLeft.useStepSize, lineTop.useStepSize, false, bufferCount);
  bufferCount += GenerateGeneric( -1, -tSize, lineLeft.useStepSize, lineBottom.useStepSize, true, bufferCount);
  bufferCount += GenerateGeneric( 1, tSize, lineRight.useStepSize, lineTop.useStepSize, true, bufferCount);
  bufferCount += GenerateGeneric( 1, -tSize, lineRight.useStepSize, lineBottom.useStepSize, false, bufferCount);

}

size_t GL4TerrainMeshCPU::Patch::GenerateGeneric(int64_t majorUnitSize,
                                                 int64_t minorUnitSize,
                                                 size_t stepSize,
                                                 size_t adjacentStepSize,
                                                 bool flip,
                                                 size_t bufferIdx)
{
  size_t count = 0;
  size_t pSize = static_cast<size_t>(patchSize);
  size_t pSize2 = (pSize - 1) / 2;

  uint32_t *iptr = &buffer[bufferIdx];

  for (size_t iz = 0; iz < pSize2; iz += stepSize)
  {
    size_t v0 = vcenter + iz * majorUnitSize;
    size_t v1 = v0 + stepSize * majorUnitSize;
    for (size_t ix = 0; ix <= iz; ix += stepSize)
    {
      size_t v00 = v0 + ix * minorUnitSize;
      size_t v01 = v00 + stepSize * minorUnitSize;
      size_t v10 = v1 + ix * minorUnitSize;
      size_t v11 = v10 + stepSize * minorUnitSize;

      bool adjacent = iz == ix;
      if (adjacent)
      {
        size_t adjacentStep = std::min(stepSize, adjacentStepSize);
        for (size_t as = 0; as < stepSize; as += adjacentStep)
        {
          size_t vas0 = v00 + as * (majorUnitSize + minorUnitSize);
          size_t vas1 = vas0 + adjacentStep * (majorUnitSize + minorUnitSize);
          if (flip)
          {
            *iptr++ = v10;
            *iptr++ = vas0;
            *iptr++ = vas1;
          }
          else
          {
            *iptr++ = v10;
            *iptr++ = vas1;
            *iptr++ = vas0;
          }
          count += 3;
        }
      }
      else
      {
        if (flip)
        {
          *iptr++ = v00;
          *iptr++ = v11;
          *iptr++ = v10;

          *iptr++ = v00;
          *iptr++ = v01;
          *iptr++ = v11;
        }
        else
        {
          *iptr++ = v00;
          *iptr++ = v10;
          *iptr++ = v11;

          *iptr++ = v00;
          *iptr++ = v11;
          *iptr++ = v01;
        }

        count += 6;
      }

    }
  }
  return count;
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
  for (auto &patch: m_patches)
  {
    needUpdate |= patch.UpdateIndices(m_referencePoint, m_terrainSize);
  }
  return needUpdate;
}

void GL4TerrainMeshCPU::RebuildIndices()
{
  glBindVertexArray(0);

  m_ib->Bind();
  size_t offset = 0;
  m_indexBufferSize = 0;
  for (auto &patch: m_patches)
  {
    size_t patchSize = sizeof(uint32_t) * patch.bufferCount;
    m_ib->Copy(patch.buffer, patchSize, offset);
    offset += patchSize;
    m_indexBufferSize += patch.bufferCount;
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
void GL4TerrainMeshGeneratorCPU::SetSize(const Vector3f &min, const Vector3f &max)
{
  m_min = min;
  m_max = max;
}
void GL4TerrainMeshGeneratorCPU::SetNormalizedHeightData(const std::vector<float> &heightData)
{
  m_heightData = heightData;

  float min = m_heightData[0];
  float max = m_heightData[0];

  for (float v: m_heightData)
  {
    min = min < v ? min : v;
    max = max > v ? max : v;
  }

  float d = max - min;
  if (d != 0.0f)
  {
    for (float &heightValue: m_heightData)
    {
      heightValue = (heightValue - min) / d;
    }
  }
}

void GL4TerrainMeshGeneratorCPU::SetHeightData(const std::vector<float> &heightData)
{
  m_heightData = heightData;
}

void GL4TerrainMeshGeneratorCPU::GenerateVerticesUVs(std::vector<Vector3f> &vertices, std::vector<Vector2f> &uv)
{
  auto size = static_cast<size_t>(m_size);
  Vector3f d = m_max - m_min;


  for (size_t iz = 0, idx = 0; iz < size; iz++)
  {
    float nz = (float) iz / (float) (size - 1);
    float z = m_min.z + nz * d.z;
    for (size_t ix = 0; ix < size; ix++, idx++)
    {
      float nx = (float) ix / (float) (size - 1);
      float x = m_min.x + nx * d.x;

      float ny = m_heightData[idx];
      float y = m_min.y + ny * d.y;

      vertices[idx] = Vector3f(x, y, z);
      uv[idx] = Vector2f(nx, nz);
    }
  }
}

void GL4TerrainMeshGeneratorCPU::GenerateNormals(std::vector<Vector3f> &vertices,
                                                 std::vector<Vector3f> &normals)
{
  auto size = static_cast<size_t>(m_size);

  for (size_t iz = 0, idx = 0; iz < size; iz++)
  {
    bool haveBottom = iz != 0;
    bool haveTop = iz != (size - 1);
    for (size_t ix = 0; ix < size; ix++, idx++)
    {
      bool haveLeft = ix != 0;
      bool haveRight = ix != (size - 1);

      const Vector3f &v = vertices[idx];
      const Vector3f &vl = haveLeft ? vertices[idx - 1] : vertices[idx];
      const Vector3f &vr = haveRight ? vertices[idx + 1] : vertices[idx];
      const Vector3f &vt = haveTop ? vertices[idx + size] : vertices[idx];
      const Vector3f &vb = haveBottom ? vertices[idx - size] : vertices[idx];

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

//      normals[idx] = Vector3f(0.0f, 1.0f, 0.0f); //n.Normalized();
      normals[idx] = n.Normalized();
    }
  }
}

void GL4TerrainMeshGeneratorCPU::EvalLine(std::vector<Vector3f> &vertices,
                                          size_t v0,
                                          size_t v1,
                                          size_t stepSize,
                                          size_t patchSize,
                                          GL4TerrainMeshCPU::Line &line)
{
  Vector3f min = vertices[v0];
  Vector3f max = vertices[v0];
  for (size_t i = v0; i <= v1; i += stepSize)
  {
    Vector3f &v = vertices[i];
    min.x = min.x < v.x ? min.x : v.x;
    min.y = min.y < v.y ? min.y : v.y;
    min.z = min.z < v.z ? min.z : v.z;
    max.x = max.x > v.x ? max.x : v.x;
    max.y = max.y > v.y ? max.y : v.y;
    max.z = max.z > v.z ? max.z : v.z;
  }
  line.v0 = v0;
  line.v1 = v1;
  line.min = min;
  line.max = max;
  line.center = (min + max) / 2.0f;
  line.absStepSize = 1;
  line.useStepSize = 1;
  line.patchSize = patchSize;
}

void GL4TerrainMeshGeneratorCPU::GeneratePatches(std::vector<Vector3f> &vertices,
                                                 std::vector<GL4TerrainMeshCPU::Patch> &patches)
{
  auto patchSize = static_cast<size_t>(m_patchSize);
  auto terrainSize = static_cast<size_t>(m_size);
  size_t numPatches = (static_cast<size_t>(m_size) - 1) / (static_cast<size_t>(m_patchSize) - 1);

  for (size_t i = 0, idx = 0; i < numPatches; ++i)
  {
    size_t v0 = i * (patchSize - 1) * terrainSize;
    size_t v1 = v0 + (patchSize - 1) * terrainSize;

    for (size_t j = 0; j < numPatches; ++j, ++idx)
    {
      size_t v00 = v0 + j * (patchSize - 1);
      size_t v01 = v00 + patchSize-1;
      size_t v10 = v1 + j * (patchSize-1);
      size_t v11 = v10 + (patchSize-1);

      size_t halfPatchSize = (patchSize - 1) / 2;
      size_t center = v00 + halfPatchSize * terrainSize + halfPatchSize;

      GL4TerrainMeshCPU::Patch &patch = patches[idx];
      EvalLine(vertices, v00, v01, 1, patchSize, patch.lineBottom);
      EvalLine(vertices, v10, v11, 1, patchSize, patch.lineTop);
      EvalLine(vertices, v00, v10, terrainSize, patchSize, patch.lineLeft);
      EvalLine(vertices, v01, v11, terrainSize, patchSize, patch.lineRight);

      patch.first = i == 0 && j == 0;
      patch.patchSize = m_patchSize;
      patch.vcenter = center;
      patch.v00 = v00;
      patch.v01 = v01;
      patch.v10 = v10;
      patch.v11 = v11;
      patch.buffer = nullptr;
    }
  }
}

iTerrainMesh *GL4TerrainMeshGeneratorCPU::Generate()
{
  auto size = static_cast<size_t>(m_size);
  size_t vertexCount = size * size;
  if (vertexCount > m_heightData.size())
  {
    return nullptr;
  }
  size_t numPatches = (static_cast<size_t>(m_size) - 1) / (static_cast<size_t>(m_patchSize) - 1);

  std::vector<Vector3f> vertices(vertexCount);
  std::vector<Vector3f> normals(vertexCount);
  std::vector<Vector2f> uvs(vertexCount);
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
  for (auto &v: vertices)
  {
    bbox.Add(v);
  }
  bbox.Finish();

  // generate the vertices
  auto vBuffer = std::vector<float>(8 * vertexCount);
  float *vptr = vBuffer.data();
  for (size_t i = 0; i < vertexCount; ++i)
  {
    const Vector3f vertex = vertices[i];
    const Vector3f normal = normals[i];
    const Vector2f uv = uvs[i];
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
  vb->Copy(vBuffer.data(), vertexCount * vertexSize);

  // initialize an empty index buffer
  auto ib = new GL4IndexBuffer;
  ib->Bind();
  ib->CreateForRendering((size - 1) * (size - 1) * 6 * 4);


  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


  ib->Bind();
  vb->Bind();

  for (auto &attribute: attributes)
  {
    glVertexAttribPointer(
        attribute.Location,
        attribute.Size,
        GL_FLOAT,
        false,
        attribute.Stride,
        reinterpret_cast<const void *>(attribute.Offset)
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
