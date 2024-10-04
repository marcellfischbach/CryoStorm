
#pragma once


#include <ceCore/csCoreExport.hh>
#include <ceCore/graphics/iRenderMesh.hh>
#include <ceCore/csTypes.hh>

namespace cryo
{


struct iTexture2D;

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



CS_CLASS()
struct CS_CORE_API iTerrainMesh : public CS_SUPER(iRenderMesh)
{
CS_CLASS_GEN;
  ~iTerrainMesh() override = default;



  virtual void SetReferencePoint(const csVector3f& pos) = 0;
};


CS_CLASS()
struct CS_CORE_API iTerrainMeshGenerator : CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iTerrainMeshGenerator() override = default;

  virtual void SetSize (eTerrainSize size) = 0;
  virtual void SetPatchSize (eTerrainSize size) = 0;
  virtual void SetSize(const csVector3f &min, const csVector3f& max) = 0;
  virtual void SetNormalizedHeightData (const std::vector<float> &heightData) = 0;
  virtual void SetHeightData (const std::vector<float> &heightData) = 0;


  CS_NODISCARD virtual iTerrainMesh* Generate() = 0;
};


CS_CLASS()
struct CS_CORE_API iTerrainMeshGeneratorFactory : CS_SUPER(iObject)
{
CS_CLASS_GEN;
  ~iTerrainMeshGeneratorFactory() override = default;

  CS_NODISCARD virtual iTerrainMeshGenerator* Create() = 0;
};



}