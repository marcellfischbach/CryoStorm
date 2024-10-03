#pragma once
#include <ceCore/coreexport.hh>
#include <ceCore/graphics/scene/igfxscene.hh>


namespace cryo
{

CS_CLASS()
class GfxOctreeScene : public CS_SUPER(iGfxScene)
{
  CS_CLASS_GEN_OBJECT;
public:
  GfxOctreeScene();
  ~GfxOctreeScene () override = default;

  void Add(GfxCamera *camera) override;
  void Remove(GfxCamera *camera) override;

  void Add(GfxMesh *sceneMesh) override;
  void Remove(GfxMesh *sceneMesh) override;

  void Add(GfxLight *light) override;
  void Remove(GfxLight *light) override;

  CS_NODISCARD const std::vector<GfxCamera *> &GetCameras() const override;

  void ScanMeshes(const iClipper *clipper, GfxSceneCollector &collector) const override;
  void ScanMeshes(const iClipper *clipper, uint32_t scanMask, GfxSceneCollector &collector) const override;

  void ScanMeshes(const iClipper *clipper,
                  uint32_t scanMask,
                  const std::function<void(GfxMesh *)> &callback
                 ) const override;

  void ScanLights(const iClipper *clipper,
                  uint32_t scanMask,
                  const std::function<bool(GfxLight *)> &callback
                 ) const override;


private:
  std::vector<GfxCamera*> m_cameras;
};


}
