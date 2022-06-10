
#include <ceOpenGL/gl4/gl4terrainmesh.hh>


namespace ce::opengl
{

GL4TerrainMeshGeneratorCPU::GL4TerrainMeshGeneratorCPU()
  : iTerrainMeshGenerator()
  ,
{
  CE_CLASS_GEN_CONSTR;
}

void GL4TerrainMeshGeneratorCPU::SetSize(eTerrainSize size)
{

}
void GL4TerrainMeshGeneratorCPU::SetPatchSize(eTerrainSize size)
{

}
void GL4TerrainMeshGeneratorCPU::SetSize(const Vector3f& min, const Vector3f& max)
{

}
void GL4TerrainMeshGeneratorCPU::SetNormalizedHeightData(const std::vector<float>& heightData)
{

}

iTerrainMesh* GL4TerrainMeshGeneratorCPU::Generate()
{
  return nullptr;
}
GL4TerrainMeshGeneratorFactory::GL4TerrainMeshGeneratorFactory()
  : iTerrainMeshGeneratorFactory()
{
  CE_CLASS_GEN_CONSTR;
}

iTerrainMeshGenerator* GL4TerrainMeshGeneratorFactory::Create()
{
  return new GL4TerrainMeshGeneratorCPU();
}

}
