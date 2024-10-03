

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/eRenderPass.hh>
#include <ceCore/graphics/scene/csGfxMesh.hh>
#include <ceCore/graphics/scene/iGfxScene.hh>
#include <ceCore/math/iclipper.hh>
#include <vector>
#include <functional>

namespace cryo
{

struct iDevice;
class csGfxLight;
class csGfxCamera;
struct LightInfluenceOnMesh;


CS_CLASS()
class CS_CORE_API GfxSimpleScene : public CS_SUPER(iGfxScene)
{
  friend struct iRenderPipeline;
  CS_CLASS_GEN_OBJECT;
public:

  GfxSimpleScene();

  void Add(csGfxCamera *camera) override;
  void Remove(csGfxCamera *camera) override;
  void Add(csGfxMesh * sceneMesh) override;
  void Remove(csGfxMesh * sceneMesh) override;
  void Add(csGfxLight* light) override;
  void Remove(csGfxLight* light) override;

  CS_NODISCARD const std::vector<csGfxCamera*> &GetCameras() const override;


  void ScanMeshes(const iClipper *clipper, csGfxSceneCollector &collector) const override;
  void ScanMeshes(const iClipper *clipper, uint32_t scanMask, csGfxSceneCollector &collector) const override;
  void ScanMeshes(const iClipper* clipper, uint32_t scanMask, const std::function<void(csGfxMesh*)> &callback) const override;
  void ScanLights(const iClipper* clipper, uint32_t scanMask, const std::function<bool(csGfxLight*)> &callback) const override;


private:
  static void Add(csGfxLight *light, std::vector<csGfxLight*> &lights);
  static void Remove(csGfxLight *light, std::vector<csGfxLight*> &lights);

  void ScanGlobalLights(const std::function<bool(csGfxLight *)> &callback) const;
  void ScanStaticLights(const iClipper* clipper, const std::function<bool(csGfxLight *)> &callback) const;
  void ScanDynamicLights(const iClipper* clipper, const std::function<bool(csGfxLight *)> &callback) const;

  std::vector<csGfxCamera*> m_cameras;

  std::vector<csGfxMesh*> m_dynamicMeshes;
  std::vector<csGfxMesh*> m_staticMeshes;
  std::vector<csGfxMesh*> m_unshadedMeshes;

  std::vector<csGfxLight*> m_globalLights;
  std::vector<csGfxLight*> m_staticLights;
  std::vector<csGfxLight*> m_dynamicLights;
};

}

