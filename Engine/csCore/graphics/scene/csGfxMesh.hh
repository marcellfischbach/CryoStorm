
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/math/csBoundingBox.hh>
#include <csCore/math/csMatrix4f.hh>
#include <csCore/graphics/eRenderPass.hh>
#include <csCore/graphics/scene/csGfxLight.hh>
#include <csCore/csRef.hh>
#include <vector>

namespace cs
{

struct iDevice;
struct iMaterial;
struct iRenderMesh;
class csSkeleton;

CS_CLASS()
class CS_CORE_API csGfxMesh : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  struct CS_CORE_API Light
  {
    csRef<csGfxLight> light;
    float      influence;
    bool operator==(const csGfxLight *l) const
    { return light == l; }

    Light(csGfxLight* light = nullptr, float influence = 0.0f);
  };


  csGfxMesh();
  ~csGfxMesh() override;

  void SetBatchable(bool batchable);
  CS_NODISCARD bool IsBatchable() const;

  void SetStatic(bool _static);
  CS_NODISCARD bool IsStatic() const;

  void SetReceiveShadow(bool receiveShadow);
  CS_NODISCARD bool IsReceiveShadow() const;
  void SetCastShadow(bool castShadow);
  CS_NODISCARD bool IsCastShadow() const;

  void SetMaterial(iMaterial *material);
  CS_NODISCARD iMaterial *GetMaterial();
  CS_NODISCARD const iMaterial *GetMaterial() const;

  void SetMesh(iRenderMesh *mesh);
  iRenderMesh *GetMesh();
  CS_NODISCARD const iRenderMesh *GetMesh() const;

  void SetModelMatrix(const csMatrix4f &modelMatrix);
  CS_NODISCARD const csMatrix4f &GetModelMatrix() const;

  void ClearLights();
  void AddLight(csGfxLight *light, float incluence);
  void RemoveLight(csGfxLight *light);
  void SortAndLimitLights(Size size);
  CS_NODISCARD Size GetNumberOfLights() const;
  CS_NODISCARD const std::vector<Light> &GetLights() const;

  CS_NODISCARD const csBoundingBox &GetBoundingBox() const;

  void Render(iDevice *device, eRenderPass pass);
  void RenderUnlit(iDevice *device, eRenderPass pass);
  void RenderForward(iDevice *device, eRenderPass pass, const csGfxLight **lights, Size numberOfLights);

  void SetFrame(uint64_t frame);
  CS_NODISCARD uint64_t GetFrame() const;

  void SetLightingDirty(bool lightingDirty);
  CS_NODISCARD bool IsLightingDirty() const;

  void SetSkeleton(csSkeleton *skeleton);
  CS_NODISCARD csSkeleton *GetSkeleton();
  CS_NODISCARD const csSkeleton *GetSkeleton() const;


private:
  void UpdateBoundingBox();

  bool m_batchable     = false;
  bool m_static        = false;
  bool m_receiveShadow = true;
  bool               m_castShadow    = true;
  csAssetRef<iMaterial> m_material;
  csRef<iRenderMesh> m_mesh     = nullptr;
  csMatrix4f    m_modelMatrix;
  csBoundingBox m_boundingBox;
  uint64_t      m_frame         = 0;
  bool        m_lightingDirty = true;

  std::vector<Light> m_lights;
  csRef<csSkeleton> m_skeleton;

};

}