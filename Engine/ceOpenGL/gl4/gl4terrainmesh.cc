
#include <ceOpenGL/gl4/gl4terrainmesh.hh>
#include <ceOpenGL/gl4/gl4terrainmesh_cpu.hh>


namespace ce::opengl
{


GL4TerrainMeshGeneratorFactory::GL4TerrainMeshGeneratorFactory()
  : iTerrainMeshGeneratorFactory()
{
  CS_CLASS_GEN_CONSTR;
}

iTerrainMeshGenerator* GL4TerrainMeshGeneratorFactory::Create()
{
  return new GL4TerrainMeshGeneratorCPU();
}

}
