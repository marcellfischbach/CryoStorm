
#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/math/boundingbox.hh>
#include <spcCore/math/matrix4f.hh>
#include <spcCore/graphics/erenderpass.hh>
#include <spcCore/graphics/scene/gfxlight.hh>
#include <vector>

namespace spc
{

struct iDevice;
struct iMaterial;
struct iRenderMesh;

SPC_CLASS()
class SPC_CORE_API GfxMesh : public SPC_SUPER(iObject)
{
SPC_CLASS_GEN_OBJECT;
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

  void SetStatic(bool _static);
  SPC_NODISCARD bool IsStatic() const;

  void SetReceiveShadow (bool receiveShadow);
  SPC_NODISCARD bool IsReceiveShadow () const;
  void SetCastShadow(bool castShadow);
  SPC_NODISCARD bool IsCastShadow() const;

  void SetMaterial(iMaterial* material);
  SPC_NODISCARD iMaterial* GetMaterial();
  SPC_NODISCARD const iMaterial* GetMaterial() const;

  void SetMesh(iRenderMesh* mesh);
  iRenderMesh* GetMesh();
  SPC_NODISCARD const iRenderMesh* GetMesh() const;

  void SetModelMatrix(const Matrix4f& modelMatrix);
  SPC_NODISCARD const Matrix4f& GetModelMatrix() const;

  void ClearLights();
  void AddLight(GfxLight* light, float incluence);
  void RemoveLight(GfxLight* light);
  void SortAndLimitLights(Size size);
  SPC_NODISCARD Size GetNumberOfLights() const;
  SPC_NODISCARD const std::vector<Light>& GetLights() const;

  SPC_NODISCARD const BoundingBox& GetBoundingBox() const;

  void Render(iDevice* device, eRenderPass pass);
  void RenderUnlit(iDevice* device, eRenderPass pass);
  void RenderForward(iDevice* device, eRenderPass pass, const GfxLight** lights, Size numberOfLights);

  void SetFrame(uint64_t frame);
  SPC_NODISCARD uint64_t GetFrame() const;

  void SetLightingDirty(bool lightingDirty);
  SPC_NODISCARD bool IsLightingDirty() const;


private:
  void UpdateBoundingBox();

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