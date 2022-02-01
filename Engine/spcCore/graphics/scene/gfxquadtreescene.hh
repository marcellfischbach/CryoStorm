
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/graphics/scene/igfxscene.hh>


namespace spc
{



SPC_CLASS()
class GfxQuadtreeScene : public SPC_SUPER(iGfxScene)
{
  SPC_CLASS_GEN_OBJECT;

public:
  GfxQuadtreeScene();
  ~GfxQuadtreeScene() override = default;

  void Add(GfxCamera * camera) override;
  void Remove(GfxCamera * camera) override;

  void Add(GfxMesh * sceneMesh) override;
  void Remove(GfxMesh * sceneMesh) override;

  void Add(GfxLight * light) override;
  void Remove(GfxLight * light) override;

  SPC_NODISCARD const std::vector<GfxCamera*>& GetCameras() const override;

  void ScanMeshes(const iClipper * clipper,
    uint32_t scanMask,
    const std::function<void(GfxMesh*)> &callback
  ) const override;
  void ScanLights(const iClipper * clipper,
    uint32_t scanMask,
    const std::function<bool(GfxLight*)> &callback
  ) const override;

private:
  struct Cell;

  static void Add(GfxLight *light, std::vector<GfxLight*> &lights);
  static void Remove(GfxLight *light, std::vector<GfxLight*> &lights);

  void ScanGlobalLights (const std::function<bool(GfxLight *)> &callback) const;
  void ScanStaticLights (const iClipper* clipper, const std::function<bool(GfxLight *)> &callback) const;
  void ScanDynamicLights (const iClipper* clipper, const std::function<bool(GfxLight *)> &callback) const;

  std::vector<GfxCamera*> m_cameras;
  std::vector<GfxMesh*> m_shadedMeshes;
  std::vector<GfxMesh*> m_unshadedMeshes;

  std::vector<GfxLight*> m_globalLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_dynamicLights;

  Cell* m_root;
};


}
