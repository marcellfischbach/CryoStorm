
#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/csTypes.hh>

namespace cs
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



CS_CLASS(jclass="org.cryo.core.graphics.ITerrainMesh")
struct CS_CORE_API iTerrainMesh : public CS_SUPER(iRenderMesh)
{
CS_CLASS_GEN;
  ~iTerrainMesh() override = default;

  CS_FUNCTION()
  virtual void SetReferencePoint(const cs::csVector3f& pos) = 0;
};


CS_CLASS(jclass="org.cryo.core.graphics.ITerrainMeshGenerator")
struct CS_CORE_API iTerrainMeshGenerator : CS_SUPER(iObject)
{
CS_CLASS_GEN;

  ~iTerrainMeshGenerator() override = default;


  CS_FUNCTION(jenum)
  virtual void SetSize (cs::eTerrainSize size) = 0;
  CS_FUNCTION(jenum)
  virtual void SetPatchSize (cs::eTerrainSize size) = 0;
  CS_FUNCTION()
  virtual void SetSize(const cs::csVector3f &min, const cs::csVector3f& max) = 0;
  CS_FUNCTION()
  virtual void SetNormalizedHeightData (const std::vector<float> &heightData) = 0;
  CS_FUNCTION()
  virtual void SetHeightData (const std::vector<float> &heightData) = 0;


  CS_FUNCTION()
  CS_NODISCARD virtual cs::csOwned<cs::iTerrainMesh> Generate() = 0;
};


CS_CLASS(jclass="org.cryo.core.graphics.ITerrainMeshGeneratorFactory")
struct CS_CORE_API iTerrainMeshGeneratorFactory : CS_SUPER(iObject)
{
CS_CLASS_GEN;
  ~iTerrainMeshGeneratorFactory() override = default;

  CS_FUNCTION()
  CS_NODISCARD virtual cs::csOwned<cs::iTerrainMeshGenerator> Create() = 0;
};



}