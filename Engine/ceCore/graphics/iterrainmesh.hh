
#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/graphics/irendermesh.hh>

namespace ce
{

enum class eTerrainSize
{
  TS_33     = 33,
  TS_65     = 65,
  TS_129    = 129,
  TS_257    = 257,
  TS_513    = 513,
  TS_1025   = 1025,
  TS_2049   = 2049,
  TS_4097   = 4097,
};

CE_CLASS()
struct CE_CORE_API iTerrainMesh : public CE_SUPER(iRenderMesh)
{
CE_CLASS_GEN;
  ~iTerrainMesh() override = default;



  virtual void SetReferencePoint (const Vector3f& pos) = 0;
};


CE_CLASS()
struct CE_CORE_API iTerrainMeshGenerator : CE_SUPER(iObject)
{
CE_CLASS_GEN;

  ~iTerrainMeshGenerator() override = default;

  virtual void SetSize (eTerrainSize size) = 0;
  virtual void SetPatchSize (eTerrainSize size) = 0;
  virtual void SetSize (const Vector3f &min, const Vector3f& max) = 0;
  virtual void SetNormalizedHeightData (const std::vector<float> &heightData) = 0;


  CE_NODISCARD virtual iTerrainMesh* Generate() = 0;
};


CE_CLASS()
struct CE_CORE_API iTerrainMeshGeneratorFactory : CE_SUPER(iObject)
{
CE_CLASS_GEN;
  ~iTerrainMeshGeneratorFactory() override = default;

  CE_NODISCARD virtual iTerrainMeshGenerator* Create() = 0;
};



}