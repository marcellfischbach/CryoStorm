
#include <csOpenGL/gl4/csGL4TerrainMeshGeneratorFactory.hh>
#include <csOpenGL/gl4/csGL4TerrainMeshCPU.hh>


namespace cryo::opengl
{


csGL4TerrainMeshGeneratorFactory::csGL4TerrainMeshGeneratorFactory()
  : iTerrainMeshGeneratorFactory()
{
  CS_CLASS_GEN_CONSTR;
}

iTerrainMeshGenerator* csGL4TerrainMeshGeneratorFactory::Create()
{
  return new csGL4TerrainMeshGeneratorCPU();
}

}
