
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/boundingbox.hh>
#include <ceCore/math/matrix4f.hh>
#include <ceCore/graphics/erenderpass.hh>
#include <ceCore/graphics/scene/gfxlight.hh>
#include <vector>

namespace ce
{

struct iDevice;
struct iMaterial;
struct iRenderMesh;

CE_CLASS()
class CE_CORE_API GfxMesh : public CE_SUPER(iObject)
{
CE_CLASS_GEN_OBJECT;
public:
  struct Light
  {
    GfxLight* Light;
    float Influence;
    bool operator==(const GfxLight* l) const
    { return Light == l; }
  };


  GfxMesh();
  ~GfxMesh() override;

  void SetBatchable (bool batchable);
  CE_NODISCARD bool IsBatchable () const;

  void SetStatic(bool _static);
  CE_NODISCARD bool IsStatic() const;

  void SetReceiveShadow (bool receiveShadow);
  CE_NODISCARD bool IsReceiveShadow () const;
  void SetCastShadow(bool castShadow);
  CE_NODISCARD bool IsCastShadow() const;

  void SetMaterial(iMaterial* material);
  CE_NODISCARD iMaterial* GetMaterial();
  CE_NODISCARD const iMaterial* GetMaterial() const;

  void SetMesh(iRenderMesh* mesh);
  iRenderMesh* GetMesh();
  CE_NODISCARD const iRenderMesh* GetMesh() const;

  void SetModelMatrix(const Matrix4f& modelMatrix);
  CE_NODISCARD const Matrix4f& GetModelMatrix() const;

  void ClearLights();
  void AddLight(GfxLight* light, float incluence);
  void RemoveLight(GfxLight* light);
  void SortAndLimitLights(Size size);
  CE_NODISCARD Size GetNumberOfLights() const;
  CE_NODISCARD const std::vector<Light>& GetLights() const;

  CE_NODISCARD const BoundingBox& GetBoundingBox() const;

  void Render(iDevice* device, eRenderPass pass);
  void RenderUnlit(iDevice* device, eRenderPass pass);
  void RenderForward(iDevice* device, eRenderPass pass, const GfxLight** lights, Size numberOfLights);

  void SetFrame(uint64_t frame);
  CE_NODISCARD uint64_t GetFrame() const;

  void SetLightingDirty(bool lightingDirty);
  CE_NODISCARD bool IsLightingDirty() const;


private:
  void UpdateBoundingBox();

  bool m_batchable = false;
  bool m_static = false;
  bool m_receiveShadow = true;
  bool m_castShadow = true;
  iMaterial* m_material = nullptr;
  iRenderMesh* m_mesh = nullptr;
  Matrix4f m_modelMatrix;
  BoundingBox m_boundingBox;
  uint64_t  m_frame = 0;
  bool m_lightingDirty = true;

  std::vector<Light> m_lights;


};

}