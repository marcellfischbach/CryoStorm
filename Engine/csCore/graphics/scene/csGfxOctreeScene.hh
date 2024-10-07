#pragma once
#include <csCore/csCoreExport.hh>
#include <csCore/graphics/scene/iGfxScene.hh>


namespace cs
{

CS_CLASS()
class csGfxOctreeScene : public CS_SUPER(iGfxScene)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGfxOctreeScene();
  ~csGfxOctreeScene() override = default;

  void Add(csGfxCamera *camera) override;
  void Remove(csGfxCamera *camera) override;

  void Add(csGfxMesh *sceneMesh) override;
  void Remove(csGfxMesh *sceneMesh) override;

  void Add(csGfxLight *light) override;
  void Remove(csGfxLight *light) override;

  CS_NODISCARD const std::vector<csGfxCamera *> &GetCameras() const override;

  void ScanMeshes(const iClipper *clipper, csGfxSceneCollector &collector) const override;
  void ScanMeshes(const iClipper *clipper, uint32_t scanMask, csGfxSceneCollector &collector) const override;

  void ScanMeshes(const iClipper *clipper,
                  uint32_t scanMask,
                  const std::function<void(csGfxMesh *)> &callback
                 ) const override;

  void ScanLights(const iClipper *clipper,
                  uint32_t scanMask,
                  const std::function<bool(csGfxLight *)> &callback
                 ) const override;


private:
  std::vector<csGfxCamera*> m_cameras;
};


}
