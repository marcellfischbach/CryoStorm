
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/iTerrainMesh.hh>

namespace cs::opengl
{


CS_CLASS()
class csGL4TerrainMeshGeneratorFactory : public CS_SUPER(iTerrainMeshGeneratorFactory)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4TerrainMeshGeneratorFactory();
  virtual ~csGL4TerrainMeshGeneratorFactory() override = default;

private:
  iTerrainMeshGenerator* Create() override;
};

}