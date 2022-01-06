

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
class GfxCamera;
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
    eSM_Dynamic = 4,
    eSM_Unshaded = 8,
  };

  GfxScene();

  void Add(GfxCamera *camera);
  void Remove(GfxCamera *camera);
  void Add(GfxMesh * sceneMesh);
  void Remove(GfxMesh * sceneMesh);
  void Add(GfxLight* light);
  void Remove(GfxLight* light);

  const std::vector<GfxCamera*> &GetCameras () const;


  void ScanMeshes(const iClipper* clipper, uint32_t scanMask, const std::function<void(GfxMesh*)> &callback) const;
  void ScanLights(const iClipper* clipper, uint32_t scanMask, const std::function<bool(GfxLight*)> &callback) const;


private:
  static void Add(GfxLight *light, std::vector<GfxLight*> &lights);
  void Remove(GfxLight *light, std::vector<GfxLight*> &lights);

  void ScanGlobalLights (const std::function<bool(GfxLight *)> &callback) const;
  void ScanStaticLights (const iClipper* clipper, const std::function<bool(GfxLight *)> &callback) const;
  void ScanDynamicLights (const iClipper* clipper, const std::function<bool(GfxLight *)> &callback) const;

  std::vector<GfxCamera*> m_cameras;

  std::vector<GfxMesh*> m_dynamicMeshes;
  std::vector<GfxMesh*> m_staticMeshes;
  std::vector<GfxMesh*> m_unshadedMeshes;

  std::vector<GfxLight*> m_globalLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_dynamicLights;
};

}

