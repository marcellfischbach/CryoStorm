
#include <csOpenGL/gl4/csGL4TerrainMeshGeneratorFactory.hh>
#include <csOpenGL/gl4/csGL4TerrainMeshCPU.hh>


namespace cs::opengl
{


csGL4TerrainMeshGeneratorFactory::csGL4TerrainMeshGeneratorFactory(bool compat)
  : iTerrainMeshGeneratorFactory()
  , m_compat(compat)
{

}

csOwned<iTerrainMeshGenerator> csGL4TerrainMeshGeneratorFactory::Create()
{
  return new csGL4TerrainMeshGeneratorCPU(m_compat);
}

}
