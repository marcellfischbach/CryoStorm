//
// Created by Marcell on 19.12.2024.
//

#include <csCore/graphics/csDefaultMeshGenerators.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/csObjectRegistry.hh>


namespace cs
{


csCubeMeshGenerator::csCubeMeshGenerator()
    : m_halfExtends(0.5f, 0.5f, 0.5f)
    , m_smooth(false)
    , m_generateTexCoords(true)
    , m_texScale(1.0f, 1.0f)
{

}


csCubeMeshGenerator &csCubeMeshGenerator::HalfExtends(const cs::csVector3f &halfExtends)
{
  m_halfExtends = halfExtends;
  return *this;
}

csCubeMeshGenerator &csCubeMeshGenerator::Smooth(bool smooth)
{
  m_smooth = smooth;
  return *this;
}

csCubeMeshGenerator &csCubeMeshGenerator::GenerateTexCoords(bool generateTexCoords)
{
  m_generateTexCoords = generateTexCoords;
  return *this;
}


csCubeMeshGenerator &csCubeMeshGenerator::TexScale(const cs::csVector2f &texScale)
{
  m_texScale = texScale;
  return *this;
}

csOwned<iRenderMesh> csCubeMeshGenerator::Generate()
{
  return m_smooth
         ? GenerateSmooth()
         : GenerateFlat();
}

csOwned<iRenderMesh> csCubeMeshGenerator::GenerateSmooth()
{
  return nullptr;
}

csOwned<iRenderMesh> csCubeMeshGenerator::GenerateFlat()
{

  auto generatorFactory = csObjectRegistry::Get<iRenderMeshGeneratorFactory>();
  if (!generatorFactory)
  {
    return nullptr;
  }

  auto gen = generatorFactory->Create();
  if (!gen)
  {
    return nullptr;
  }

  csVector3f hl = m_halfExtends;


  uint32_t o = 0;

  m_vertices.clear();
  m_normals.clear();
  m_texCoords.clear();
  m_indices.clear();

  //
  // left face
  AddFlatFaceVertices(csVector3f(-hl.x, -hl.y, hl.z),
                      csVector3f(-hl.x, hl.y, hl.z),
                      csVector3f(-hl.x, hl.y, -hl.z),
                      csVector3f(-hl.x, -hl.y, -hl.z)
  );
  AddFlatFaceNormals(csVector3f(-1.0f, 0.0, 0.0));
  AddFaceTexCoords();
  AddFlatFaceIndices(o);

  //
  // right face
  AddFlatFaceVertices(csVector3f(hl.x, -hl.y, -hl.z),
                      csVector3f(hl.x, hl.y, -hl.z),
                      csVector3f(hl.x, hl.y, hl.z),
                      csVector3f(hl.x, -hl.y, hl.z)
  );
  AddFlatFaceNormals(csVector3f(1.0f, 0.0, 0.0));
  AddFaceTexCoords();
  AddFlatFaceIndices(o);


  //
  // bottom face
  AddFlatFaceVertices(csVector3f(-hl.x, -hl.y, hl.z),
                      csVector3f(-hl.x, -hl.y, -hl.z),
                      csVector3f(hl.x, -hl.y, -hl.z),
                      csVector3f(hl.x, -hl.y, hl.z)
  );
  AddFlatFaceNormals(csVector3f(0.0f, -1.0, 0.0));
  AddFaceTexCoords();
  AddFlatFaceIndices(o);

  //
  // top face
  AddFlatFaceVertices(csVector3f(-hl.x, hl.y, -hl.z),
                      csVector3f(-hl.x, hl.y, hl.z),
                      csVector3f(hl.x, hl.y, hl.z),
                      csVector3f(hl.x, hl.y, -hl.z)
  );
  AddFlatFaceNormals(csVector3f(0.0f, 1.0, 0.0));
  AddFaceTexCoords();
  AddFlatFaceIndices(o);

  //
  // front face
  AddFlatFaceVertices(csVector3f(-hl.x, -hl.y, -hl.z),
                      csVector3f(-hl.x, hl.y, -hl.z),
                      csVector3f(hl.x, hl.y, -hl.z),
                      csVector3f(hl.x, -hl.y, -hl.z)
  );
  AddFlatFaceNormals(csVector3f(0.0f, 0.0, -1.0));
  AddFaceTexCoords();
  AddFlatFaceIndices(o);

  //
  // back face
  AddFlatFaceVertices(csVector3f(hl.x, -hl.y, hl.z),
                      csVector3f(hl.x, hl.y, hl.z),
                      csVector3f(-hl.x, hl.y, hl.z),
                      csVector3f(-hl.x, -hl.y, hl.z)
  );
  AddFlatFaceNormals(csVector3f(0.0f, 0.0, 1.0));
  AddFaceTexCoords();
  AddFlatFaceIndices(o);

  gen->SetVertices(m_vertices);
  gen->SetNormals(m_normals);
  if (m_generateTexCoords)
  {
    gen->SetUV0(m_texCoords);
  }
  gen->SetIndices(m_indices);

  return gen->Generate();
}

void
csCubeMeshGenerator::AddFlatFaceVertices(const cs::csVector3f &v0,
                                         const cs::csVector3f &v1,
                                         const cs::csVector3f &v2,
                                         const cs::csVector3f &v3)
{
  m_vertices.push_back(v0);
  m_vertices.push_back(v1);
  m_vertices.push_back(v2);
  m_vertices.push_back(v3);
}

void csCubeMeshGenerator::AddFlatFaceNormals(const cs::csVector3f &normal)
{
  m_normals.push_back(normal);
  m_normals.push_back(normal);
  m_normals.push_back(normal);
  m_normals.push_back(normal);
}

void csCubeMeshGenerator::AddFaceTexCoords()
{
  m_texCoords.emplace_back(0.0f, 0.0f);
  m_texCoords.emplace_back(0.0f, m_texScale.y);
  m_texCoords.emplace_back(m_texScale.x, m_texScale.y);
  m_texCoords.emplace_back(m_texScale.x, 0.0f);
}

void csCubeMeshGenerator::AddFlatFaceIndices(cs::uint32_t &offset)
{
  m_indices.push_back(offset + 0);
  m_indices.push_back(offset + 1);
  m_indices.push_back(offset + 2);
  m_indices.push_back(offset + 0);
  m_indices.push_back(offset + 2);
  m_indices.push_back(offset + 3);
  offset += 4;
}

}