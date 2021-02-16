

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/math/iclipper.hh>
#include <vector>
#include <functional>

namespace spc
{

struct iDevice;
class GfxLight;
struct LightInfluenceOnMesh;


SPC_CLASS()
class SPC_CORE_API GfxScene : public SPC_SUPER(iObject)
{
  friend struct iRenderPipeline;
  SPC_CLASS_GEN_OBJECT;
public:

  enum eScanMask
  {
    eSM_Global = 1,
    eSM_Static = 2,
    eSM_Dynamic = 3
  };

  GfxScene();
  
  void Add(GfxMesh * sceneMesh);
  void Remove(GfxMesh * sceneMesh);
  void Add(GfxLight* light);
  void Remove(GfxLight* light);


  void Render(iDevice * device, eRenderPass pass);

  void ScanMeshes(const iClipper* clipper, UInt32 scanMask, std::function<void(GfxMesh*)> callback) const;
  void ScanLights(const iClipper* clipper, UInt32 scanMask, std::function<bool(GfxLight*)> callback) const;

private:
  void Add(GfxLight *light, std::vector<GfxLight*> &lights);
  void Remove(GfxLight *light, std::vector<GfxLight*> &lights);


  void AddStaticLightsToMesh(GfxMesh *mesh);
  void AddStaticLightToMeshes(GfxLight *light);
  float CalcMeshLightInfluence( GfxLight* light, const GfxMesh* mesh);
  std::vector<GfxMesh::Light> CalcMeshLightInfluences(const GfxMesh * mesh, const std::vector<GfxLight*> &lights);


  std::vector<GfxMesh*> m_dynamicMeshes;
  std::vector<GfxMesh*> m_staticMeshes;

  std::vector<GfxLight*> m_globalLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_dynamicLights;
};

}

