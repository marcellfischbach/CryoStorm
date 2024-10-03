
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceCore/graphics/iterrainmesh.hh>

namespace cryo::opengl
{


CS_CLASS()
class GL4TerrainMeshGeneratorFactory : public CS_SUPER(iTerrainMeshGeneratorFactory)
{
CS_CLASS_GEN_OBJECT;
public:
  GL4TerrainMeshGeneratorFactory();
  virtual ~GL4TerrainMeshGeneratorFactory() override = default;

private:
  iTerrainMeshGenerator* Create() override;
};

}