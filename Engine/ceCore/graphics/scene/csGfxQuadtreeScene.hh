
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/scene/iGfxScene.hh>


namespace cryo
{


CS_CLASS()
class csGfxQuadtreeScene : public CS_SUPER(iGfxScene)
{
CS_CLASS_GEN_OBJECT;

public:
  csGfxQuadtreeScene();
  ~csGfxQuadtreeScene() override = default;

  void Add(csGfxCamera *camera) override;
  void Remove(csGfxCamera *camera) override;

  void Add(csGfxMesh *sceneMesh) override;
  void Remove(csGfxMesh *sceneMesh) override;

  void Add(csGfxLight *light) override;
  void Remove(csGfxLight *light) override;

  CS_NODISCARD const std::vector<csGfxCamera *> &GetCameras() const override;

  void Optimize() override;

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
  struct Cell;

  static void Add(csGfxLight *light, std::vector<csGfxLight *> &lights);
  static void Remove(csGfxLight *light, std::vector<csGfxLight *> &lights);

  void ScanGlobalLights(const std::function<bool(csGfxLight *)> &callback) const;
  void ScanStaticLights(const iClipper *clipper, const std::function<bool(csGfxLight *)> &callback) const;
  void ScanDynamicLights(const iClipper *clipper, const std::function<bool(csGfxLight *)> &callback) const;

  std::vector<csGfxCamera *> m_cameras;
  std::vector<csGfxMesh *>   m_shadedMeshes;
  std::vector<csGfxMesh *>   m_unshadedMeshes;

  std::vector<csGfxLight *> m_globalLights;
  std::vector<csGfxLight *> m_staticLights;
  std::vector<csGfxLight *> m_dynamicLights;

  Cell *m_root;
};


}
