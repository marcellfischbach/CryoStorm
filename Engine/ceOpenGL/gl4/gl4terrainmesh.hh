
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/iterrainmesh.hh>

namespace ce::opengl
{


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