#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/defs.hh>
#include <functional>

namespace cryo
{

class csGfxCamera;
class csGfxMesh;
class csGfxLight;
struct iClipper;
class csGfxSceneCollector;

CS_CLASS()
struct CS_CORE_API iGfxScene : public CS_SUPER(iObject)
{
CS_CLASS_GEN;


  enum eScanMask
  {
    eSM_Global   = 1,
    eSM_Static   = 2,
    eSM_Dynamic  = 4,
    eSM_Unshaded = 8,
  };


  ~iGfxScene() override = default;


  virtual void Add(csGfxCamera *camera) = 0;

  virtual void Remove(csGfxCamera *camera) = 0;

  virtual void Add(csGfxMesh *sceneMesh) = 0;

  virtual void Remove(csGfxMesh *sceneMesh) = 0;

  virtual void Add(csGfxLight *light) = 0;

  virtual void Remove(csGfxLight *light) = 0;

  CS_NODISCARD virtual const std::vector<csGfxCamera *> &GetCameras() const = 0;

  virtual void Optimize () {}

  virtual void ScanMeshes(const iClipper *clipper,
                          csGfxSceneCollector &collector
                         ) const = 0;
  virtual void ScanMeshes(const iClipper *clipper,
                          uint32_t scanMask,
                          csGfxSceneCollector &collector
                          ) const = 0;

  virtual void ScanMeshes(const iClipper *clipper,
                          uint32_t scanMask,
                          const std::function<void(csGfxMesh *)> &callback
                         ) const = 0;

  virtual void ScanLights(const iClipper *clipper,
                          uint32_t scanMask,
                          const std::function<bool(csGfxLight *)> &callback
                         ) const = 0;

};


}