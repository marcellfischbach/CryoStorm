#pragma once
#include <spcCore/coreexport.hh>
#include <spcCore/graphics/scene/igfxscene.hh>


namespace spc
{

SPC_CLASS()
class GfxOctreeScene : public SPC_SUPER(iGfxScene)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GfxOctreeScene();
  ~GfxOctreeScene () override = default;

  void Add(GfxCamera *camera) override;
  void Remove(GfxCamera *camera) override;

  void Add(GfxMesh *sceneMesh) override;
  void Remove(GfxMesh *sceneMesh) override;

  void Add(GfxLight *light) override;
  void Remove(GfxLight *light) override;

  SPC_NODISCARD const std::vector<GfxCamera *> &GetCameras() const override;

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
