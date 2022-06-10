
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/iterrainmesh.hh>

namespace ce::opengl
{

class CE_OGL_API GL4TerrainMeshCPU : public CE_SUPER(iTerrainMesh)
{

};

CE_CLASS()
class GL4TerrainMeshGeneratorCPU : public CE_SUPER(iTerrainMeshGenerator)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4TerrainMeshGeneratorCPU();
  ~GL4TerrainMeshGeneratorCPU() override = default;

  void SetSize(eTerrainSize size) override;
  void SetPatchSize(eTerrainSize size) override;
  void SetSize(const Vector3f& min, const Vector3f& max) override;
  void SetNormalizedHeightData(const std::vector<float>& heightData) override;

  iTerrainMesh* Generate() override;
private:
  eTerrainSize m_size = eTerrainSize::TS_1025;
  eTerrainSize m_pathSize = eTerrainSize::TS_65;
  Vector3f m_min = Vector3f(-1000.0f, 0.0f, -1000.0f);
  Vector3f m_max = Vector3f(1000.0f, 100.0f, 1000.0f);
};

CE_CLASS()
class GL4TerrainMeshGeneratorFactory : public CE_SUPER(iTerrainMeshGeneratorFactory)
{
CE_CLASS_GEN_OBJECT;
public:
  GL4TerrainMeshGeneratorFactory();
  virtual ~GL4TerrainMeshGeneratorFactory() override = default;

private:
  iTerrainMeshGenerator* Create() override;
};

}